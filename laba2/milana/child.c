#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int func(int number1, int number2, int number3) {

    if (number2 != 0 || number3 != 0) {
        return 1;
    }  
}

int main(int argc, char *argv[]){

    int pipefd[2];
    pipefd[0] = atoi(argv[1]);
    pipefd[1] = atoi(argv[2]);
    int file = atoi(argv[0]);
    if (dup2(file, 0) == -1) {
        perror("error dub\n");
        return 0;
    }
    if (dup2(pipefd[1], 1) == -1) {
        perror("error dub\n");
        return 0;
    }
    float n, n1, n2;
    while (scanf("%f %f %f", &n,&n1,&n2) != EOF) {
        if (func(n,n1,n2) == 1) {
            n1 = n / n1;
            n2 = n / n2;
            printf("%f %f %f\n", n, n1,n2);
        } else {
            printf("%c", '\0');
            exit(0);
        }
    }
    printf("%c", '\0');
    close(pipefd[0]);
    close(pipefd[1]);

}
