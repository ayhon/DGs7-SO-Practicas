#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
/*
 * CUESTION A:
 *
 * Soluciona la implementación inicial, manteniendo la escritura 
 * concurrente en el fichero. Es decir, el proceso padre escribirá
 * los cinco ceros iniciales, el hijo uno los cinco unos, etc, 
 * sin necesidad de sincronizar los procesos. Es decir, se desea 
 * que no sea necesario imponer un orden en la ejecución de los 
 * procesos.
 *
 * */

int main(void)
{
    int fd1,fd2,i,pos;
    char c;
    char buffer[6];

    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    write(fd1, "00000", 5);
    for (i=1; i < 10; i++) {
        if (fork() == 0) {
            /* Child */
            sprintf(buffer, "%d", i*11111);
			fd2 = open("output.txt", O_WRONLY);
            lseek(fd2, 5*i, SEEK_SET);
            write(fd2, buffer, 5);
            close(fd2);
            exit(0);
        }
    }

    //wait for all children to finish
    while (wait(NULL) != -1);

    lseek(fd1, 0, SEEK_SET);
    printf("File contents are:\n");
    while (read(fd1, &c, 1) > 0)
        printf("%c", (char) c);
    printf("\n");
    close(fd1);
    exit(0);
}
