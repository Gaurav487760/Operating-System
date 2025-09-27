#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child Process: PID = %d\n", getpid());
        printf("Child executing 'ls' using execl()\n");
        execl("/bin/ls", "ls", "-l", NULL);
        perror("execl failed");
    } else if (pid > 0) {
        printf("Parent Process: PID = %d\n", getpid());
        printf("Parent going to sleep...\n");
        sleep(10);
        printf("Parent waking up after sleep.\n");
    } else {
        perror("fork failed");
    }
    return 0;
}

