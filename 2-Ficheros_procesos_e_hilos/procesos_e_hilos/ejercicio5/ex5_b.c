#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
/*
 * CUESTION B:
 *
 * Proponer una solución en la que el padre escriba su número
 * entre la escritura de los hijos, de modo que el contenido del
 * fichero al final será:
 *
 *  000001111100000222220000033333000004444400000555550000066666000007777700000888880000099999
 *
 *
 * */

int main(void)
{
    int fd1,fd2,i,pos;
    char c;
    char buffer[6];

    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    /* write(fd1, "00000", 5); */
    for (i=1; i < 10; i++) {
        if (fork() == 0) {
            /* Child */
            sprintf(buffer, "%d", i*11111);
			fd2 = open("output.txt", O_WRONLY);
            lseek(fd2, 5*(2*i-1), SEEK_SET);
            write(fd2, buffer, 5);
            close(fd2);
            exit(0);
        }
    }
    for (i=1; i < 10; i++) {
        lseek(fd1, 5*(2*i-2), SEEK_SET);
		write(fd1,"00000",5);
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

