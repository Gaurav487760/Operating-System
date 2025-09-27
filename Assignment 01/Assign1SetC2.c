#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>

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

void list_directory(const char *option, const char *dirname) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Failed to open directory");
        exit(1);
    }

    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (strcmp(option, "f") == 0) {
            printf("%s\n", entry->d_name);
        } else if (strcmp(option, "i") == 0) {
            printf("File: %-20s Inode: %lu\n", entry->d_name, (unsigned long)entry->d_ino);
        }

        count++;
    }

    if (strcmp(option, "n") == 0) {
        printf("Total entries (excluding . and ..): %d\n", count);
    }

    closedir(dir);
}

int main() {
    char input[MAX_CMD];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell$ ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';  // Remove newline

        if (strcmp(input, "exit") == 0) {
            break;
        }

        int arg_count = 0;
        char *token = strtok(input, " ");
        while (token && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        if (arg_count == 3 && strcmp(args[0], "count") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                count_file(args[1], args[2]);
                exit(0);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("fork failed");
            }

        } else if (arg_count == 3 && strcmp(args[0], "list") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                list_directory(args[1], args[2]);
                exit(0);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("fork failed");
            }

        } else {
            printf("Unsupported command. Try:\n");
            printf("  count [c|w|l] filename\n");
            printf("  list [f|n|i] dirname\n");
            printf("  exit\n");
        }
    }

    return 0;
}

