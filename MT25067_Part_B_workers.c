/*
 * Part B: Worker Functions
 * calibrated for measurable execution time ~5-15 seconds
 */

#include "MT25067_Part_B_workers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

long get_iterations() {
    return ROLL_LAST_DIGIT * BASE_ITERATIONS; // Returns 7000
}

// 1. CPU: Needs MORE work to be visible
void run_cpu_intensive() {
    long count = get_iterations();
    double result = 0.0;
    // Increased multiplier from 1000 to 100,000 to catch 'top' attention
    for (long i = 0; i < count * 100000; i++) { 
        result += sin(i) * cos(i);
    }
    if (result == 12345.0) printf("Ignore\n"); 
}

// 2. MEM: Needs LESS work to avoid 17-minute runtime
void run_mem_intensive() {
    long count = get_iterations();
    // Reduced buffer from 50MB to 1MB (1024*1024)
    // 7000 * 1MB = 7GB total throughput (manageable in ~5-10s)
    size_t size = 1 * 1024 * 1024; 
    char *buffer = (char *)malloc(size);
    if (!buffer) return;

    for (long i = 0; i < count; i++) {
        memset(buffer, i % 255, size);
        // Volatile access to ensure it's not optimized away
        volatile char c = buffer[i % size];
        (void)c; 
    }
    free(buffer);
}

// 3. IO: Needs LESS work to avoid disk thrashing
void run_io_intensive() {
    long count = get_iterations();
    FILE *fp;
    const char *filename = "temp_io_test.txt";
    char data[] = "Writing data.\n";

    for (long i = 0; i < count; i++) {
        fp = fopen(filename, "w");
        if (fp) {
            // Reduced writes per file open from 100 to 10
            for(int j=0; j<10; j++) { 
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
    remove(filename);
}