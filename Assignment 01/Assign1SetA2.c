#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {"ls", "-l", NULL}; // No full path needed
        printf("Child process PID: %d\n", getpid());
        printf("Replacing child process with 'ls' using execvp()\n");
        execvp("ls", args);
        perror("execvp failed");
    } else if (pid > 0) {
        // Parent process
        printf("Parent process PID: %d\n", getpid());
        wait(NULL);
        printf("Child finished. Parent resumes.\n");
    } else {
        perror("fork failed");
    }
    return 0;
}

