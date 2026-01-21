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
#include <pthread.h>

long get_iterations() {
    return ROLL_LAST_DIGIT * BASE_ITERATIONS; // Returns 7000
}

// helper function
double calculate_pi_leibniz(long terms) {
    double pi = 0.0;
    for (long i = 0; i < terms; i++) {
        double term = 4.0 / (2 * i + 1);
        if (i % 2 == 0) pi += term;
        else pi -= term;
    }
    return pi;
}

// 1. CPU: Needs MORE work to be visible
void run_cpu_intensive() {
    long count = get_iterations(); // 7000
    double dummy_result = 0.0;

    for (long i = 0; i < count; i++) { 
        // Perform a heavy calculation: Compute Pi to 3,000,000 terms precision
        dummy_result += calculate_pi_leibniz(3000000);
    }

    // Prevent compiler optimization
    if (dummy_result == 123.456) printf("Ignore\n");
}

// 2. MEM: Needs LESS work to avoid 17-minute runtime
void run_mem_intensive() {
    long count = get_iterations();
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
    char filename[50];
    snprintf(filename, sizeof(filename), "io_test_%lx.txt", (unsigned long)pthread_self());    
    
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