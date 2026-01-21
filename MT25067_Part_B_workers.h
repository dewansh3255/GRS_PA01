#ifndef WORKERS_H
#define WORKERS_H

#include <stddef.h>

#define ROLL_LAST_DIGIT 7
#define BASE_ITERATIONS 1000 

// Worker function prototypes
void run_cpu_intensive();
void run_mem_intensive();
void run_io_intensive();

// Helper to get iteration count
long get_iterations();

#endif