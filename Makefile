# Makefile for PA01
# Replace MT25xxx with your actual roll number in the variables below

CC = gcc
CFLAGS = -Wall -Wextra -pthread
ROLL_NO = MT25067

# Source files
WORKER_SRC = $(ROLL_NO)_Part_B_workers.c
PROG_A_SRC = $(ROLL_NO)_Part_A_Program_A.c
PROG_B_SRC = $(ROLL_NO)_Part_A_Program_B.c

# Executables
EXEC_A = Program_A
EXEC_B = Program_B

all: $(EXEC_A) $(EXEC_B)

$(EXEC_A): $(PROG_A_SRC) $(WORKER_SRC)
	$(CC) $(CFLAGS) -o $(EXEC_A) $(PROG_A_SRC) $(WORKER_SRC) -lm

$(EXEC_B): $(PROG_B_SRC) $(WORKER_SRC)
	$(CC) $(CFLAGS) -o $(EXEC_B) $(PROG_B_SRC) $(WORKER_SRC) -lm

clean:
	rm -f $(EXEC_A) $(EXEC_B) *.o