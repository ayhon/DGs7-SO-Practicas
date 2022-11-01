#include <stdio.h>
#include <fcntl.h>  // For open
#include <unistd.h> // For read, write, etc
#include <error.h>

// CONSTANTS
#define N_ARGS 2
#define BUFFER_SIZE 512

// MACROS
#define error_out(...) error(1,0,__VA_ARGS__)

typedef struct {
	int fd;
	char* name;
} FileInfo;

FileInfo create_file_info(char* filename, int open_flags){
	FileInfo res;
	res.fd = open(filename, open_flags,0640);
	if(res.fd == -1) error_out("Couldn't open file `%s`",filename);
	res.name = filename;
	return res;
}

// TODO: Does it matter that it errors if argv[1] == argv[2]?
int main(int argc, char** argv){
	if (argc < 2)        error_out("Must provide file to copy");
	if (argc < 3)        error_out("Must provide path of new file");
	if (argc > N_ARGS+1) error_out("Exceeded the maximum number of arguments: %ui",N_ARGS);

	FileInfo from = create_file_info(argv[1], O_RDONLY);
	/* char* file_name = argv[1]; */
	/* int file_fd = open(file_name, O_RDONLY); */
	/* if (file_fd == -1) error_out("Couldn't open file `%s`",file_name); */

	FileInfo to = create_file_info(argv[2],O_TRUNC | O_CREAT | O_WRONLY);
	/* char* new_file_name = argv[2]; */
	/* int new_file_fd = open(new_file_name, O_TRUNC | O_CREAT | O_WRONLY); */
	/* if (new_file_fd == -1) error_out("Couldn't open file `%s`",new_file_name); */

	int bytes_read;
	char* buffer[BUFFER_SIZE];
	/* while((bytes_read = read(file_fd, &buffer, BUFFER_SIZE)) != 0) */
	/* 	if(write(new_file_fd, &buffer, bytes_read) != bytes_read) */
	/* 		error_out("Couldn't write contents to file `%s`",new_file_name); */
	while((bytes_read = read(from.fd, &buffer, BUFFER_SIZE)) != 0)
		if(write(to.fd, &buffer, bytes_read) != bytes_read)
			error_out("Couldn't write contents to file `%s`",to.name);
	
	if (close(to.fd) == -1)
		error_out("Couldn't close file `%s`", to.name);
	if (close(from.fd) == -1)
		error_out("Couldn't close file `%s`", from.name);
	/* if (close(new_file_fd) == -1) */
	/* 	error_out("Couldn't close file `%s`", new_file_name); */
	/* if (close(file_fd) == -1) */
	/* 	error_out("Couldn't close file `%s`", file_name); */

	return 0;
}
