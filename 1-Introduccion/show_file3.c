#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int ret;

	if (argc < 2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	size_t block_size = 1;
	if (argc >= 3)
		block_size = (size_t)atoi(argv[2]);

	char* c = calloc(block_size,sizeof(char)); 
	/// Must be char* or it segfaults if block_size is too large
	/// You have to give it a size with malloc, or it'll overflow


	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	int num_read;
	do {
		num_read = fread(c, sizeof(char), block_size, file);
		int ret = fwrite(c, sizeof(char), num_read, stdout);
		if (ferror(file)) { // Why does this fail?
			fclose(file);
			free(c);
			err(3,"fwrite() failed!!");
		}
	} while (!feof(file) && num_read == block_size);

	fclose(file);
	free(c);
	return 0;
}
