#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void parse_input(char *input, char **args, int *background);
void execute_command(char **args, int background);
void handle_pipes(char **args);
void handle_redirection(char **args);
int is_builtin_command(char **args);
void execute_builtin_command(char **args);
void cd_command(char **args);
void print_prompt();

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];
    int background;

    while (1) {
        print_prompt();  // Display the shell prompt
        fflush(stdout);
        if (!fgets(input, MAX_INPUT_SIZE, stdin)) {  // Read input from the user
            break;
        }

        if (strlen(input) == 1) {
            continue;  // Ignore empty input
        }

        input[strlen(input) - 1] = '\0';  // Remove the newline character

        parse_input(input, args, &background);  // Parse the input into arguments

        if (args[0] == NULL) {
            continue;  // Ignore empty command
        }

        if (is_builtin_command(args)) {
            execute_builtin_command(args);  // Execute built-in command
        } else {
            execute_command(args, background);  // Execute external command
        }
    }

    return 0;
}

void print_prompt() {
    printf("myshell> ");
}

void parse_input(char *input, char **args, int *background) {
    char *token = strtok(input, " ");
    int i = 0;
    *background = 0;

    while (token != NULL) {
        if (strcmp(token, "&") == 0) {  // Check for background execution
            *background = 1;
            break;
        }
        args[i++] = token;  // Store each argument
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null-terminate the argument list
}

void execute_command(char **args, int background) {
    pid_t pid = fork();  // Create a child process
    if (pid == 0) {  // Child process
        handle_pipes(args);  // Handle pipes if any
        handle_redirection(args);  // Handle redirection if any
        if (execvp(args[0], args) == -1) {  // Execute the command
            perror("exec");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {  // Parent process
        if (!background) {
            wait(NULL);  // Wait for the child process to complete if not running in background
        }
    }
}

void handle_pipes(char **args) {
    int i, j, fd[2], pipe_count = 0;
    char *command[MAX_ARG_SIZE][MAX_ARG_SIZE];
    pid_t pid;

    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_count++;  // Count the number of pipes
        }
    }

    if (pipe_count == 0) {
        return;  // No pipes to handle
    }

    for (i = 0, j = 0; i <= pipe_count; i++) {
        int k = 0;
        while (args[j] != NULL && strcmp(args[j], "|") != 0) {
            command[i][k++] = args[j++];  // Split commands based on pipes
        }
        command[i][k] = NULL;
        j++;
    }

    for (i = 0; i < pipe_count; i++) {
        pipe(fd);  // Create a pipe
        if ((pid = fork()) == 0) {
            dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to the pipe
            close(fd[0]);
            close(fd[1]);
            if (execvp(command[i][0], command[i]) == -1) {
                perror("exec");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        dup2(fd[0], STDIN_FILENO);  // Redirect stdin to the pipe
        close(fd[1]);
    }

    if (execvp(command[i][0], command[i]) == -1) {  // Execute the last command
        perror("exec");
        exit(EXIT_FAILURE);
    }
}

void handle_redirection(char **args) {
    int i;
    int fd;
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {  // Input redirection
            if ((fd = open(args[i + 1], O_RDONLY)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);  // Redirect stdin to the file
            close(fd);
            args[i] = NULL;
        } else if (strcmp(args[i], ">") == 0) {  // Output redirection (overwrite)
            if ((fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);  // Redirect stdout to the file
            close(fd);
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {  // Output redirection (append)
            if ((fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);  // Redirect stdout to the file
            close(fd);
            args[i] = NULL;
        }
    }
}

int is_builtin_command(char **args) {
    // Check if the command is a built-in command
    if (strcmp(args[0], "cd") == 0 || strcmp(args[0], "exit") == 0) {
        return 1;
    }
    return 0;
}

void execute_builtin_command(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        cd_command(args);  // Execute the built-in cd command
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);  // Exit the shell
    }
}

void cd_command(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: expected argument\n");  // Print error if no argument provided
    } else {
        if (chdir(args[1]) != 0) {  // Change directory
            perror("cd");
        }
    }
}
