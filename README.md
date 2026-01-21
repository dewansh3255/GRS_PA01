# PA01: Processes and Threads

**Roll Number:** MT25067  
**Course:** Graduate Systems (CSE638)  
**Assignment:** PA01 - Processes and Threads Analysis

## Overview

This assignment explores the performance characteristics of processes vs threads when executing CPU-intensive, memory-intensive, and I/O-intensive workloads. The project implements worker functions in C, measures system metrics, and analyzes scalability behavior.

## Project Structure

```
MT25067_PA01/
├── MT25067_Part_A_Program_A.c    # Process-based implementation (fork)
├── MT25067_Part_A_Program_B.c    # Thread-based implementation (pthread)
├── MT25067_Part_B_workers.c      # Worker function implementations
├── MT25067_Part_B_workers.h      # Worker function headers
├── MT25067_Part_C_shell.sh       # Automation script for Part C
├── MT25067_Part_D_shell.sh       # Automation script for Part D
├── MT25067_Part_C_CSV.csv        # Measurement results for Part C
├── MT25067_Part_D_CSV.csv        # Measurement results for Part D
├── MT25067_Part_D_plot.py        # Python script for generating plots
├── MT25067_Report.pdf            # Detailed analysis report
├── Makefile                      # Build configuration
├── Dockerfile                    # Docker environment setup
├── .gitignore                    # Git ignore rules
└── README.md                     # This file
```

## Implementation Details

### Part A: Process and Thread Creation

1. **Program A (Processes):** Creates N child processes using `fork()` (default: 2)
2. **Program B (Threads):** Creates N threads using `pthread` (default: 2)

Both programs accept command-line arguments to specify:
- Worker function type: `cpu`, `mem`, or `io`
- Number of workers (optional, for Part D scalability tests)

### Part B: Worker Functions

Based on roll number last digit (7), iteration count = 7 × 1000 = 7000

1. **CPU-Intensive (`run_cpu_intensive`):**
   - Performs 7000 × 100,000 iterations of trigonometric calculations
   - Uses `sin()` and `cos()` operations to stress the CPU
   - Calibrated for ~60-70 seconds execution time

2. **Memory-Intensive (`run_mem_intensive`):**
   - Allocates 1MB buffers and performs 7000 iterations
   - Uses `memset()` and volatile reads to prevent optimization
   - Total throughput: ~7GB of memory operations

3. **I/O-Intensive (`run_io_intensive`):**
   - Performs 7000 file open/write/read/close cycles
   - 10 writes per iteration to a temporary file
   - Tests disk I/O subsystem performance

### Part C: Basic Performance Measurement

The `MT25067_Part_C_shell.sh` script automates:
- Compilation using `make`
- Execution of all 6 program+worker combinations
- Monitoring with `top` (CPU usage) and `iostat` (disk I/O)
- CPU pinning using `taskset -c 0` for consistent results
- CSV output generation with execution time, CPU%, and disk I/O metrics

### Part D: Scalability Analysis

The `MT25067_Part_D_shell.sh` script tests:
- **Program A (Processes):** 2, 3, 4, 5 processes
- **Program B (Threads):** 2, 3, 4, 5, 6, 7, 8 threads

Focuses on the CPU-intensive workload to highlight parallelism characteristics when pinned to a single core.

## Building and Running

### Prerequisites

```bash
# Install required tools (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential sysstat procps bc
```

### Using Docker (Recommended)

```bash
# Build the Docker image
docker build -t pa01-env .

# Run the container with mounted volume
docker run -it -v $(pwd):/app pa01-env

# Inside container, run make
make
```

### Compilation

```bash
# Build all programs
make

# Clean build artifacts
make clean
```

### Manual Execution

```bash
# Run Program A with CPU task (2 processes)
taskset -c 0 ./Program_A cpu

# Run Program B with memory task (3 threads)
taskset -c 0 ./Program_B mem 3

# Run Program A with I/O task (4 processes)
taskset -c 0 ./Program_A io 4
```

### Automated Measurement

```bash
# Run Part C measurements (6 combinations)
bash MT25067_Part_C_shell.sh

# Run Part D scalability tests
bash MT25067_Part_D_shell.sh

# Generate plots (requires Python with pandas, matplotlib, seaborn)
python3 MT25067_Part_D_plot.py
```

## Results Summary

### Part C: Key Observations

| Program   | Task | Time (s) | CPU% | Disk I/O (KB/s) |
|-----------|------|----------|------|-----------------|
| Program_A | cpu  | 67.99    | 33.4 | 5.0             |
| Program_A | mem  | 0.21     | 43.8 | 242.5           |
| Program_A | io   | 1.35     | 7.2  | 121.2           |
| Program_B | cpu  | 68.54    | 99.8 | 3.8             |
| Program_B | mem  | 0.21     | 93.3 | 239.8           |
| Program_B | io   | 1.35     | 21.0 | 119.9           |

**Key Findings:**
- **Threads (Program B)** show higher CPU utilization for CPU-intensive tasks (~99% vs ~33%)
- **Processes (Program A)** distribute load across multiple CPU cores when not pinned
- **Memory tasks** complete fastest (<1 second) with high disk cache activity
- **I/O tasks** are limited by disk speed, showing low CPU usage

### Part D: Scalability Observations

When pinned to a single CPU core:
- **Processes:** Linear time increase (68s → 171s for 2→5 processes)
- **Threads:** Linear time increase (69s → 277s for 2→8 threads)
- Both show near-100% CPU utilization when threads/processes > 1
- Context switching overhead becomes apparent with increased worker count

## File Naming Conventions

All files follow the strict naming convention: `MT25067_Part_<X>_<Description>.<ext>`

- Programs: `MT25067_Part_A_Program_A.c`, `MT25067_Part_A_Program_B.c`
- Workers: `MT25067_Part_B_workers.c`, `MT25067_Part_B_workers.h`
- Scripts: `MT25067_Part_C_shell.sh`, `MT25067_Part_D_shell.sh`
- Data: `MT25067_Part_C_CSV.csv`, `MT25067_Part_D_CSV.csv`
- Report: `MT25067_Report.pdf`

## AI Usage Declaration

This project utilized AI assistance (Claude/ChatGPT) for:
- Shell script automation logic and error handling
- CSV parsing and averaging calculations in bash
- Python plotting code structure and seaborn configuration
- Dockerfile setup for reproducible environment
- README documentation formatting and structure

**All code has been reviewed and understood** by the student. The student can explain every line of code and the underlying concepts during the viva examination.

## GitHub Repository

[Link to be added: https://github.com/username/GRS_PA01]

## Notes

1. All measurements were performed in a Docker container on an ARM64 architecture (Apple Silicon)
2. The `taskset -c 0` command pins processes to CPU core 0 for consistent measurements

## Author

**Student:** MT25067  
**Date:** January 2026  
**Deadline:** January 23, 2026