#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child Process PID: %d\n", getpid());
        int increment = 10;
        int ret = nice(increment);
        if (ret == -1 && errno != 0) {
            perror("nice failed");
        } else {
            printf("Child: New nice value after increment: %d\n", ret);
        }
        int prio = getpriority(PRIO_PROCESS, 0);
        printf("Child: Current priority (nice value): %d\n", prio);
        for (int i = 0; i < 5; i++) {
            printf("Child working... %d\n", i+1);
            sleep(1);
        }
    } else if (pid > 0) {
        printf("Parent Process PID: %d\n", getpid());
        int prio = getpriority(PRIO_PROCESS, 0);
        printf("Parent: Current priority (nice value): %d\n", prio);
        printf("Parent: Child finished execution.\n");
    } else {
        perror("fork failed");
    }
    return 0;
}

