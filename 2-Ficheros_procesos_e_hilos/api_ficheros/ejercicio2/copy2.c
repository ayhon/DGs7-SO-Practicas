#include <stdio.h>
#include <fcntl.h>  // For open
#include <stdlib.h>
#include <unistd.h> // For read, write, etc
#include <error.h>
#include <sys/stat.h>

// CONSTANTS
#define N_ARGS 2
#define BUFFER_SIZE 512
#define FILETYPE_REGULAR 010
#define FILETYPE_SYMLINK 012

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

void copy(FileInfo from, char* target_name){
	struct stat info;
	lstat(from.name, &info);
	/* printf("%o\n",info.st_mode); */
	int filetype = info.st_mode >> 12;
	switch(filetype){
		default:
			error_out("Can't copy file with filetype `%o`",filetype);
			break;

		case FILETYPE_SYMLINK: {
			char* link_path = malloc((info.st_size+1)*sizeof(char)); // Incluye espacio para \0
			link_path[info.st_size] = '\0';
			int bytes_read = readlink(from.name, link_path, info.st_size);
			if (bytes_read == -1 || bytes_read != info.st_size)
				error_out("Couldn't copy the symlink's path into the buffer");
			symlink(link_path, target_name);
			break;
		}
	
		case FILETYPE_REGULAR: {
			FileInfo to = create_file_info(target_name,O_TRUNC | O_CREAT | O_WRONLY);
			int bytes_read;
			char* buffer[BUFFER_SIZE];
			while((bytes_read = read(from.fd, &buffer, BUFFER_SIZE)) != 0)
				if(write(to.fd, &buffer, bytes_read) != bytes_read)
					error_out("Couldn't write contents to file `%s`",to.name);
			if (close(to.fd) == -1)
				error_out("Couldn't close file `%s`", to.name);
			break;
		}
	}
}

// TODO: Does it matter that it errors if argv[1] == argv[2]?
int main(int argc, char** argv){
	if (argc < 2)        error_out("Must provide file to copy");
	if (argc < 3)        error_out("Must provide path of new file");
	if (argc > N_ARGS+1) error_out("Exceeded the maximum number of arguments: %ui",N_ARGS);

	FileInfo from = create_file_info(argv[1], O_RDONLY);
	copy(from,argv[2]);
	
	if (close(from.fd) == -1)
		error_out("Couldn't close file `%s`", from.name);

	return 0;
}
