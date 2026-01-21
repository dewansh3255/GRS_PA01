/*
 * Part A & D: Program A
 * Creates N processes using fork() (Default N=2)
 * Usage: ./Program_A <cpu|mem|io> [num_processes]
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "MT25067_Part_B_workers.h" // Changed to your specific header

void execute_task(const char *type) {
    if (strcmp(type, "cpu") == 0) run_cpu_intensive();
    else if (strcmp(type, "mem") == 0) run_mem_intensive();
    else if (strcmp(type, "io") == 0) run_io_intensive();
    else {
        printf("Invalid argument. Use: cpu, mem, or io\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <cpu|mem|io> [num_processes]\n", argv[0]);
        return 1;
    }

    int num_children = 2; // Default for Part A [cite: 14]
    if (argc >= 3) {
        num_children = atoi(argv[2]); // Override for Part D [cite: 46]
    }

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execute_task(argv[1]);
            exit(0);
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
    }

    // Parent waits for ALL children
    for (int i = 0; i < num_children; i++) {
        wait(NULL);
    }

    return 0;
}