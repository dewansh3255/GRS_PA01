#!/bin/bash

# Configuration
ROLL_NO="MT25067"
CSV_FILE="${ROLL_NO}_Part_D_CSV.csv"

# Initialize CSV Header
echo "Program,Task,Count,Execution_Time_Sec,Avg_CPU_Usage,Avg_Disk_Write_KB" > $CSV_FILE

# Clean and compile
make clean
make

echo "Starting Part D Scalability Measurements..."

# 1. Program A (Processes): 2, 3, 4, 5 [cite: 46]
# We stick to 'cpu' task as it highlights parallelism best, 
# but you can change 'task' to 'mem' or 'io' if you prefer or run all.
# The prompt implies running the variants. Let's pick 'cpu' for clear scaling results.
TASK="cpu"

for count in {2..5}; do
    echo "Running Program_A (Processes) with $count processes..."
    
    top -b -d 1 | grep --line-buffered "Program_A" > top_log.txt &
    TOP_PID=$!
    iostat -d -k 1 > iostat_log.txt &
    IO_PID=$!

    start_time=$(date +%s.%N)
    # Pass the count as the second argument
    taskset -c 0 ./Program_A $TASK $count
    end_time=$(date +%s.%N)

    kill $TOP_PID 2>/dev/null
    kill $IO_PID 2>/dev/null

    duration=$(echo "$end_time - $start_time" | bc)
    avg_cpu=$(awk '{sum+=$9; count++} END {if (count > 0) print sum/count; else print 0}' top_log.txt)
    avg_disk=$(grep -E "sda|vda|xvda" iostat_log.txt | awk '{sum+=$4; count++} END {if (count > 0) print sum/count; else print 0}')

    echo "Program_A,$TASK,$count,$duration,$avg_cpu,$avg_disk" >> $CSV_FILE
    sleep 1
done

# 2. Program B (Threads): 2, 3, 4, 5, 6, 7, 8 [cite: 50]
for count in {2..8}; do
    echo "Running Program_B (Threads) with $count threads..."
    
    top -b -d 1 | grep --line-buffered "Program_B" > top_log.txt &
    TOP_PID=$!
    iostat -d -k 1 > iostat_log.txt &
    IO_PID=$!

    start_time=$(date +%s.%N)
    taskset -c 0 ./Program_B $TASK $count
    end_time=$(date +%s.%N)

    kill $TOP_PID 2>/dev/null
    kill $IO_PID 2>/dev/null

    duration=$(echo "$end_time - $start_time" | bc)
    avg_cpu=$(awk '{sum+=$9; count++} END {if (count > 0) print sum/count; else print 0}' top_log.txt)
    avg_disk=$(grep -E "sda|vda|xvda" iostat_log.txt | awk '{sum+=$4; count++} END {if (count > 0) print sum/count; else print 0}')

    echo "Program_B,$TASK,$count,$duration,$avg_cpu,$avg_disk" >> $CSV_FILE
    sleep 1
done

echo "Part D Done. Results in $CSV_FILE"