#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BASE_BUF_SIZE 4

typedef struct {
    char* prompt;
} Environment;

Environment* ENV;

void init_env();
void main_loop();
void deinit_env();

char* read_user_input();

int main(int argc, char** argv) {

    init_env();

    main_loop();

    deinit_env();

    return EXIT_SUCCESS;
}

void main_loop() {
    int status;
    char* input;
    do {
        printf("%s", ENV->prompt);

        input = read_user_input();

        if (strcmp(input, "exit") == 0) {
            status = 0;
        } else {
            printf("You typed: %s\n", input);
            status = 1;
        }

        free(input);

    } while(status);
}

char* read_user_input() {
    int size = BASE_BUF_SIZE;

    char* buf = (char*) malloc(size);
    if (buf == NULL) {
        fprintf(stderr, "shsh: Allocation error\n");
        exit(EXIT_FAILURE);
    }

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
            if (buf == NULL) {
                fprintf(stderr, "shsh: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    buf[i] = '\0';

    return buf;
}

void init_env() {
    ENV = (Environment*) malloc(sizeof(Environment));
    if (ENV == NULL) {
        fprintf(stderr, "shsh: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    ENV->prompt = (char *) malloc(3);
    if (ENV->prompt == NULL) {
        fprintf(stderr, "shsh: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    strcpy(ENV->prompt, "$ ");
}

void deinit_env() {
    free(ENV->prompt);
    free(ENV);
    ENV = NULL;
}
