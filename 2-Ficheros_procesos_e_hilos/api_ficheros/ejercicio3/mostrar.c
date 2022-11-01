#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <error.h>

#define BUFF_SIZE 1
#define STDOUT 1

#define error_out(...) error(1,0,__VA_ARGS__)

/*
 *
 * Recibe el nombre de un fichero y lo
 * muestra por stdou
 * 
 * Opciones:
 *  -n N Número de bits que usar según opción `-e`
 *  -e Se leen los últimos N bytes del fichero. Si no aparece, se saltan los últimos N bytes del fichero
 *
 * */
int main(int argc, char *argv[])
{
	int n = 0, e = 0;
	char opt;
	/* printf("%i\n", getpid()); */
	do {
		opt = getopt(argc, argv, "n:e");
		switch(opt){
			case 'n':
				n = atoi(optarg);
				break;
			case 'e':
				e = 1;
				break;
			case -1:
				break;
			default:
				error_out("Couldn't recognize option `%c`",opt);
		}
	} while(opt != -1);

	char* filename = argv[optind];
	int fd;
	if ((fd = open(filename, O_RDONLY)) == -1)
		error_out("Couldn't open file `%s`", filename);

	off_t offset;
	if((offset = lseek( fd
  					  , e?       -n : n
  					  , e? SEEK_END : SEEK_SET)) == -1)
		error_out("Couldn't move the file offset");

	char buffer[BUFF_SIZE];
	int bytes_read = 0;
	int bytes_written = 0;
	while((bytes_read = read(fd, &buffer, BUFF_SIZE))){
		if((bytes_written = write(STDOUT_FILENO, &buffer, bytes_read)) != bytes_read){
			error_out("Couldn't write to STDIN");
		}
	}
	return 0;
}
