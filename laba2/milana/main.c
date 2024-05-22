#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    size_t size = 0;
    char *file_name = (char *)calloc(1, sizeof(char));
    char c;
    while ((c = getchar()) != '\n') {
        file_name[size] = c;
        file_name = (char *)realloc(file_name, (++size + 1) * sizeof(char));
    }
    file_name[size++] = '\0';
    int file = open(file_name, O_RDONLY);
    if (file == -1) {
        perror("error file\n");
        return 0;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1){
        perror("error pipe\n");
        return 0;
    }
    pid_t id = fork();
    if (id == -1) {
        perror("error fork");
        return 0;
    } else if (id == 0){
        char *argv1 = malloc(sizeof(int));
        sprintf(argv1, "%d", file);
        char *argv2 = malloc(sizeof(int));
        sprintf(argv2, "%d", pipefd[0]);
        char *argv3 = malloc(sizeof(int));
        sprintf(argv3, "%d", pipefd[1]);
        execlp("./child", argv1, argv2, argv3, NULL);
        free(argv1);
        free(argv2);
        free(argv3);
    } else {
        char p;
        while (1) {
            read(pipefd[0], &p, sizeof(p));
            if (p == '\0') {
                exit(0);
            } else {
                putchar(p);
            }
        }
    }
    free(file_name);
    close(pipefd[0]);
    close(pipefd[1]);
    close(file);
    return 0;
}