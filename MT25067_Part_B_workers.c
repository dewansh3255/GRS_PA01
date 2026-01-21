/*
 * Part B: Worker Functions
 * Implements CPU, Memory, and I/O intensive tasks.
 * Modular and well-commented as per.
 */

#include "MT25067_Part_B_workers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

long get_iterations() {
    return ROLL_LAST_DIGIT * BASE_ITERATIONS;
}

// 1. Function "cpu": CPU-intensive [cite: 19]
// Performs complex calculations (e.g., matrix multiplication or trig functions)
void run_cpu_intensive() {
    long count = get_iterations();
    double result = 0.0;
    for (long i = 0; i < count * 1000; i++) { // Added multiplier to make it measurable by 'top'
        result += sin(i) * cos(i);
    }
    // Prevent compiler optimization
    if (result == 12345.0) printf("Ignore\n"); 
}

// 2. Function "mem": Memory-intensive [cite: 22]
// Moves large amounts of data to bottleneck RAM [cite: 23]
void run_mem_intensive() {
    long count = get_iterations();
    // Allocate a large chunk (e.g., 50MB) to stress memory bandwidth
    size_t size = 50 * 1024 * 1024; 
    char *buffer = (char *)malloc(size);
    if (!buffer) return;

    for (long i = 0; i < count; i++) {
        // memset and memcpy force CPU-RAM interaction
        memset(buffer, i % 255, size);
    }
    free(buffer);
}

// 3. Function "io": I/O-intensive [cite: 26]
// Spends time waiting for disk operations [cite: 28]
void run_io_intensive() {
    long count = get_iterations();
    FILE *fp;
    const char *filename = "temp_io_test.txt";
    char data[] = "Writing some data to disk to simulate I/O load.\n";

    for (long i = 0; i < count; i++) {
        fp = fopen(filename, "w");
        if (fp) {
            for(int j=0; j<100; j++) { // Write repeatedly
                fputs(data, fp);
            }
            fclose(fp);
        }
        // Read it back
        fp = fopen(filename, "r");
        if(fp) {
            char buffer[100];
            while(fgets(buffer, 100, fp));
            fclose(fp);
        }
    }
    remove(filename); // Clean up
}