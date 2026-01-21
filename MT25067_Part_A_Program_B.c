/*
 * Part A & D: Program B
 * Creates N threads using pthread (Default N=2)
 * Usage: ./Program_B <cpu|mem|io> [num_threads]
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "MT25067_Part_B_workers.h"

void *thread_wrapper(void *arg) {
    char *type = (char *)arg;
    if (strcmp(type, "cpu") == 0) run_cpu_intensive();
    else if (strcmp(type, "mem") == 0) run_mem_intensive();
    else if (strcmp(type, "io") == 0) run_io_intensive();
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <cpu|mem|io> [num_threads]\n", argv[0]);
        return 1;
    }

    int num_threads = 2; // Default for Part A [cite: 15]
    if (argc >= 3) {
        num_threads = atoi(argv[2]); // Override for Part D [cite: 50]
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, thread_wrapper, (void *)argv[1]) != 0) {
            perror("Failed to create thread");
            free(threads);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}