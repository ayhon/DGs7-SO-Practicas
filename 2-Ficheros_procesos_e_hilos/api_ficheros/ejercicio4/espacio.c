#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <error.h>
#include <sys/types.h>
#include <dirent.h>

#define error_out(...) error(1,0,__VA_ARGS__)

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

char* new_string_ending_in_slash(char* dname){
	int n = strlen(dname), extra = 0;
	if (dname[n-1] != '/') extra = 1;
	char* basedir = malloc((n+extra+1)*sizeof(char)); // One extra for "\0"
	strcpy(basedir, dname);
	if (extra) {
		basedir[n]='/';
		basedir[n+1]='\0';
	}
	return basedir;
}

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat info;
	if(lstat(fname, &info) == -1) 
		error_out("Couldn't get info on file `%s`",fname);
	switch(info.st_mode & S_IFMT){
		case S_IFDIR:{
			char * dname = new_string_ending_in_slash(fname);
			get_size_dir(dname, blocks);
			free(dname);
			break;
		}
		default:
			*blocks += info.st_blocks>>1;
	}

	return 0;
}

char* concatenate(const char* a, const char* b){
	char* res = malloc((strlen(a)+strlen(b)+1)*sizeof(char));
	strcpy(res, a);
	return strcat(res, b);
}

/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{

	DIR* d = opendir(dname);
	struct dirent* info;
	while((info = readdir(d))) {
		int ignore_dir = strcmp(info->d_name,".") != 0 && strcmp(info->d_name,"..") != 0;
		if (ignore_dir){
			char* basedir = concatenate(dname, info->d_name);
			get_size(basedir, blocks);
			free(basedir);
		}
	}

	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++){
		size_t size = 0; 
		get_size(argv[i], &size);
		printf("%lu %s\n", size, argv[i]);
	}
	return 0;
}
