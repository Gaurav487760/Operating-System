#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child Process: PID = %d, PPID = %d\n", getpid(), getppid());
        printf("Hello World\n");
    } else {
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
        printf("Hi\n");
    }

    return 0;
}

