#!/bin/bash

# Configuration
ROLL_NO="MT25067"
CSV_FILE="${ROLL_NO}_Part_C_CSV.csv"
PROGRAMS=("Program_A" "Program_B")
TASKS=("cpu" "mem" "io")

# Initialize CSV Header
echo "Program,Task,Execution_Time_Sec,Avg_CPU_Usage,Avg_Disk_Write_KB" > $CSV_FILE

echo "Starting measurements... Output will be saved to $CSV_FILE"

# Make sure executables exist
make

for prog in "${PROGRAMS[@]}"; do
    for task in "${TASKS[@]}"; do
        echo "Running $prog with $task..."

        # Start monitoring in background
        # Record top output every 1 second. 
        # Grep filter for the program name to only get relevant CPU usage.
        top -b -d 1 | grep "$prog" > top_log.txt &
        TOP_PID=$!

        # Record iostat every 1 second
        iostat -d 1 > iostat_log.txt &
        IO_PID=$!

        # Run the program and measure time
        # We use strict formatting for time (Real time in seconds)
        start_time=$(date +%s.%N)
        ./$prog $task
        end_time=$(date +%s.%N)
        
        # Kill monitors immediately after program finishes
        kill $TOP_PID 2>/dev/null
        kill $IO_PID 2>/dev/null

        # Calculate Duration
        duration=$(echo "$end_time - $start_time" | bc)

        # Process logs to get averages
        # AWK is used to sum the %CPU column (usually col 9 in top) and divide by count
        avg_cpu=$(awk '{sum+=$9; count++} END {if (count > 0) print sum/count; else print 0}' top_log.txt)
        
        # Parse iostat for writes (kB_wrtn/s is usually col 4 or similar, verify manually if needed)
        # Assuming disk sda or similar. This extracts the average write speed.
        # Note: You might need to adjust the column number based on your system's iostat output.
        avg_disk=$(grep "sda" iostat_log.txt | awk '{sum+=$4; count++} END {if (count > 0) print sum/count; else print 0}')

        echo "Finished $prog + $task: Time=$duration, CPU=$avg_cpu, Disk=$avg_disk"
        
        # Append to CSV
        echo "$prog,$task,$duration,$avg_cpu,$avg_disk" >> $CSV_FILE

        # Clean up temp logs
        rm top_log.txt iostat_log.txt
        sleep 2 # Cool down
    done
done

echo "All Done! Check $CSV_FILE"