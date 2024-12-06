#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Function to sleep for a random duration (1-8 seconds)
void random_sleep() {
    srand(time(NULL) ^ getpid()); // Seed random with process ID
    int sleep_time = (rand() % 8) + 1; // Random number between 1 and 8
    sleep(sleep_time);
}

// Function to implement Pattern 1
void pattern1(int num) {
    for (int i = 1; i <= num; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            printf("Process %d (PID %d) beginning\n", i, getpid());
            random_sleep();
            printf("Process %d (PID %d) exiting\n", i, getpid());
            exit(0);
        } else if (pid > 0) {
            printf("Process %d (PID %d) created Process %d\n", i, getpid(), pid);
        } else {
            perror("Fork failed");
            exit(1);
        }
    }

    // Parent waits for all children
    while (wait(NULL) > 0);
}

// Function to implement Pattern 2
void pattern2(int num) {
    for (int i = 1; i <= num; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            printf("Process %d (PID %d) beginning\n", i, getpid());
            random_sleep();
            printf("Process %d (PID %d) exiting\n", i, getpid());
            exit(0);
        } else if (pid > 0) {
            printf("Process %d (PID %d) created Process %d\n", i, getpid(), pid);
            wait(NULL); // Wait for the child process to complete
        } else {
            perror("Fork failed");
            exit(1);
        }
    }
}

// Function to implement Pattern 3 (I need the extra credit)
void pattern3(int num) {
    if (num <= 0) return;

    pid_t pid1 = fork();
    if (pid1 == 0) { // First child
        printf("Process 1 (PID %d) beginning\n", getpid());
        pattern3(num - 1); // Recursive call
        printf("Process 1 (PID %d) exiting\n", getpid());
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) { // Second child
        printf("Process 2 (PID %d) beginning\n", getpid());
        pattern3(num - 1); // Recursive call
        printf("Process 2 (PID %d) exiting\n", getpid());
        exit(0);
    }

    // Parent waits for both children
    wait(NULL);
    wait(NULL);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number of things> <pattern number>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    if (num < 1 || num > 256) {
        fprintf(stderr, "Error: Number of things must be between 1 and 256.\n");
        return 1;
    }

    switch (pattern) {
        case 1:
            pattern1(num);
            break;
        case 2:
            pattern2(num);
            break;
        case 3:
            pattern3(num); // Extra credit part
            break;
        default:
            fprintf(stderr, "Error: Invalid pattern number (choose 1, 2, or 3).\n");
            return 1;
    }

    return 0;
}
