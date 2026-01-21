#!/bin/bash

# Configuration
ROLL_NO="MT25067"
CSV_FILE="${ROLL_NO}_Part_C_CSV.csv"
PROGRAMS=("Program_A" "Program_B")
TASKS=("cpu" "mem" "io")

# Initialize CSV Header
echo "Program,Task,Execution_Time_Sec,Avg_CPU_Usage,Avg_Disk_Write_KB" > $CSV_FILE

# Clean and compile
make clean
make

echo "Starting measurements... Output will be saved to $CSV_FILE"

for prog in "${PROGRAMS[@]}"; do
    for task in "${TASKS[@]}"; do
        echo "Running $prog with $task..."

        # 1. Start Logging in Background
        # 'top': -b (batch), -d 1 (delay 1s), -p (monitor specific PID later? No, grep name)
        # We filter by program name. Linux top output lines: PID USER ... %CPU ...
        top -b -d 1 | grep --line-buffered "$prog" > top_log.txt &
        TOP_PID=$!

        # 'iostat': -d (disk), -k (kB), 1 (interval). 
        # We will parse sda (or vda/simulated disk) later.
        iostat -d -k 1 > iostat_log.txt &
        IO_PID=$!

        # 2. Run Program with Taskset (Pin to CPU 0 as per Source 32)
        start_time=$(date +%s.%N)
        
        # taskset -c 0 pins the process to the first CPU core
        taskset -c 0 ./$prog $task
        
        end_time=$(date +%s.%N)

        # 3. Stop Loggers
        kill $TOP_PID 2>/dev/null
        kill $IO_PID 2>/dev/null

        # 4. Calculate Metrics
        duration=$(echo "$end_time - $start_time" | bc)

        # Parse CPU: usually column 9 in Linux top (PID, USER, PR, NI, VIRT, RES, SHR, S, %CPU)
        # We verify if column 9 looks like a number.
        avg_cpu=$(awk '{sum+=$9; count++} END {if (count > 0) print sum/count; else print 0}' top_log.txt)

        # Parse Disk: loop over iostat and look for the main disk (usually sda or vda in Docker)
        # If inside Docker, it might be 'vda' or 'overlay'. We take the max write column found.
        # Usually col 4 is kB_wrtn/s.
        avg_disk=$(grep -E "sda|vda|xvda" iostat_log.txt | awk '{sum+=$4; count++} END {if (count > 0) print sum/count; else print 0}')

        echo "Finished $prog + $task: Time=$duration, CPU=$avg_cpu, Disk=$avg_disk"
        
        # Append to CSV
        echo "$prog,$task,$duration,$avg_cpu,$avg_disk" >> $CSV_FILE

        rm top_log.txt iostat_log.txt
        sleep 1
    done
done

echo "Done. Results in $CSV_FILE"