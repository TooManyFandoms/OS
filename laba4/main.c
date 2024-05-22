#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    size_t size = 0;
    char *file_name = (char *)calloc(1, sizeof(char));
    if (file_name == NULL){
        perror("can't read file name\n");
        return 0;
    }
    char c;
    while ((c = getchar()) != '\n') {
        file_name[size] = c;
        file_name = (char *)realloc(file_name, (++size + 1) * sizeof(char));
        if (file_name == NULL){
            perror("can't read file name\n");
            return 0;
        }
    }
    file_name[size++] = '\0';
    int file = open(file_name, O_RDONLY);
    if (file == -1) {
        perror("error file\n");
        return 0;
    }
    free(file_name);
    sem_unlink("_sem");
    sem_t *sem = sem_open("_sem",  O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
    if (errno == SEM_FAILED){
        perror("error semaphor\n");
        return 0;
    }
    int state = 0;
    while (++state < 2) {   // 1
        sem_post(sem);
    }
    while (--state > 1) {
        sem_wait(sem);
    }
    sem_getvalue(sem, &state);
    pid_t id = fork();
    if (id == -1) {
        perror("error fork");
        return 0;
    } else if (id == 0) {
        sem_close(sem);
        char *argv1 = malloc(sizeof(int));
        sprintf(argv1, "%d", file);
        printf("%d", file);
        execlp("./child", argv1, NULL);
        free(argv1);
    } else {
        while (1) {
            sem_getvalue(sem, &state);
            if (state == 0) {
                int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                if (fd == -1){
                    perror("error shm_open");
                    return 0;
                }
                char *mapped = (char *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                char *allocated = (char *) malloc(sizeof(mapped));
                memcpy(allocated, mapped, sizeof(mapped));
                if (*allocated == '\0') {
                    free(allocated);
                    close(fd);
                    munmap(mapped, sizeof(int));
                    close(file);
                    sem_close(sem);
                    return 0;
                } else {
                    printf("%s\n", allocated);
                }
                free(allocated);
                close(fd);
                munmap(mapped, sizeof(int));
                sem_post(sem);
                sem_post(sem);
            }
        }
    }
    close(file);
    sem_close(sem);
    return 0;
}
