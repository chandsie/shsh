#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <unistd.h>
#include <sys/wait.h>

#define BASE_BUF_SIZE 4

typedef struct {
    char* prompt;
} Environment;

Environment* ENV;

void mem_alloc_check(void*);
void init_env();
void main_loop();
void deinit_env();

char*  read_user_input();
char** tokenize_user_input(char*);
int    execute(char**);
int    launch_proc(char**);

int main(int argc, char** argv) {

    init_env();

    main_loop();

    deinit_env();

    return EXIT_SUCCESS;
}

void main_loop() {
    int status;
    char* input;
    char** tokens;

    do {
        printf("%s", ENV->prompt);

        input = read_user_input();
        tokens = tokenize_user_input(input);
        status = execute(tokens);

        free(input);
        free(tokens);

    } while(status);
}

char* read_user_input() {
    int size = BASE_BUF_SIZE;

    char* buf = (char*) malloc(size);
    mem_alloc_check(buf);

    int i = 0;
    int c;

    while(1) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            buf[i] = '\0';
            break;
        } else {
            buf[i++] = c;
        }
        if (i >= size) {
            size *= 2;
            buf = realloc(buf, size);
            mem_alloc_check(buf);
        }
    }
    buf[i] = '\0';

    return buf;
}

char** tokenize_user_input(char* input) {
    int size = BASE_BUF_SIZE;

    char** tokens = (char**) malloc(size * sizeof(char*));
    mem_alloc_check(tokens);

    int token_count = 0;
    tokens[token_count++] = input;
    while(*input) {
        if ((*input) == ' ') {
            *input = '\0';
            tokens[token_count++] = input + 1;
        }
        input++;

        if (token_count >= size) {
            size *= 2;
            tokens = realloc(tokens, size * sizeof(char*));
            mem_alloc_check(tokens);
        }
    }

    tokens[token_count] = NULL;

    return tokens;
}

int execute(char** args) {
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    } else {
        return launch_proc(args);
    }
}

int launch_proc(char** args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("shsh");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("shsh");
        }
     } else {
         int status;
         pid_t wpid;
         do {
             wpid = waitpid(pid, &status, WUNTRACED);
         } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void init_env() {
    ENV = (Environment*) malloc(sizeof(Environment));
    mem_alloc_check(ENV);

    ENV->prompt = (char *) malloc(3);
    mem_alloc_check(ENV->prompt);

    strcpy(ENV->prompt, "$ ");
}

void deinit_env() {
    free(ENV->prompt);
    free(ENV);
    ENV = NULL;
}

void mem_alloc_check(void* mem) {
    if (mem == NULL) {
        fprintf(stderr, "shsh: Allocation error\n");
        exit(EXIT_FAILURE);
    }
}
