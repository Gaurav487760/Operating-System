#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child: My PID is %d\n", getpid());
        printf("Child: My Parent PID is %d\n", getppid());
        sleep(5);
        printf("Child (after sleep): My new Parent PID is %d\n", getppid());
        printf("Child: I am now an orphan process adopted by init/systemd.\n");
    } else if (pid > 0) {
        printf("Parent: My PID is %d\n", getpid());
        printf("Parent: Exiting immediately, child will become orphan.\n");
        exit(0); 
    } else {
        perror("fork failed");
    }

    return 0;
}

