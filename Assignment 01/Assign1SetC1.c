#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_CMD 1024
#define MAX_ARGS 10
void count_file(const char *option, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File open failed");
        exit(1);
    }
    int chars = 0, words = 0, lines = 0;
    int in_word = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        chars++;
        if (ch == '\n') lines++;
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            words++;
        }
    }
    fclose(fp);
    if (strcmp(option, "c") == 0) {
        printf("Character count: %d\n", chars);
    } else if (strcmp(option, "w") == 0) {
        printf("Word count: %d\n", words);
    } else if (strcmp(option, "l") == 0) {
        printf("Line count: %d\n", lines);
    } else {
        fprintf(stderr, "Invalid count option. Use c (chars), w (words), or l (lines).\n");
    }
}
int main() {
    char input[MAX_CMD];
    char *args[MAX_ARGS];
    while (1) {
        printf("myshell$ ");
        fflush(stdout);

        // Read user input
        if (!fgets(input, sizeof(input), stdin)) {
            break; // EOF (Ctrl+D)
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Exit shell
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Tokenize input into args
        int arg_count = 0;
        char *token = strtok(input, " ");
        while (token && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        // Handle 'count' command
        if (arg_count == 3 && strcmp(args[0], "count") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                count_file(args[1], args[2]);
                exit(0);
            } else if (pid > 0) {
                // Parent process waits
                wait(NULL);
            } else {
                perror("fork failed");
            }
        } else {
            printf("Unsupported command. Try: count [c|w|l] filename OR 'exit'\n");
        }
    }

    return 0;
}


