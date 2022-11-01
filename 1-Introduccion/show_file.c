#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* file=NULL;
    int c,ret;

    if (argc!=2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    /* Open file */
    if ((file = fopen(argv[1], "r")) == NULL)
        err(2,"The input file %s could not be opened",argv[1]);

    /* Read file byte by byte */
    if (!feof(file)) do {
		fread(&c, sizeof(char), 1, file);
        /* Print byte to stdout */
		ret=fwrite(&c, sizeof(char), 1, stdout);

        if (ret < 1){
            fclose(file);
            err(3,"putc() failed!!");
        }
    } while (!feof(file));

    fclose(file);
    return 0;
}
