#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int func(int number) {
    int composite = 0;

    if (number < 2) {
        composite = -1;
    } else {
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) {
                composite = 1;
                break;
            }
        }
        if (composite != 1) {
            composite = -1;
        }
    }
    return composite;
}

int main(int argc, char const *argv[])
{
    sem_t *sem = sem_open("_sem",  O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
    int file = atoi(argv[0]);
    if (dup2(file, 0) == -1) {
        perror("error dub\n");
        return 0;
    }
    int n, flag = 0;
    while (scanf("%d", &n) != EOF) {
        int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
        if (func(n) == 1) {
            ftruncate(fd, sizeof(int));
            char *mapped = (char *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            sprintf(mapped, "%d", n);
            munmap(mapped, sizeof(int));
            close(fd);
        } else {
            ftruncate(fd, sizeof(char));
            char *mapped = (char *) mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            sprintf(mapped, "%c", '\0');
            flag = 1;
            munmap(mapped, sizeof(char));
            close(fd);
        }
        sem_wait(sem);
        sem_wait(sem);
    }
    if (flag == 0) {
        int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
        ftruncate(fd, sizeof(char));
        char *mapped = (char *) mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf(mapped, "%c", '\0');
        munmap(mapped, sizeof(char));
        close(fd);
        sem_wait(sem);
        sem_wait(sem);
    }
    sem_close(sem);
    return 0;
}
