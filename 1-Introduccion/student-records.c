#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <getopt.h>
#define MAX_CHARS_NIF  9

typedef struct {
	int student_id; 
	char NIF[MAX_CHARS_NIF+1];  
	char* first_name;
	char* last_name;
} student_t;


/*

Además de -h, el programa implementará opciones para crear (-c) y listar (-l) ficheros de registros de estudiantes, así como para poder añadir nuevos registros al final de un fichero existente -a, o realizar búsquedas (queries) de registros específicos (-q) por identificador de estudiante (-i) o NIF -n. En el caso de las opciones -c y -a, será preciso indicar en la línea de comando una lista de registros de estudiantes a almacenar en el fichero. Cada registro de la lista especificará los campos de cada estudiante mediante una cadena de caracteres con elementos separados por ":". Por ejemplo, considérese el siguiente comando para crear un nuevo fichero de estudiantes llamado database que almacenará 2 registros:

*/
const char entry_fmt[] = "[Entry #%i]\n\tstudent_id=%i\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n";
const char usage_fmt[] = "Usage: %s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n";
char* exec_name = NULL;
void usage(int is_error){
	if (exec_name != NULL)
		fprintf(is_error?stderr:stdin, usage_fmt, exec_name);
}

// Esta función acepta como parámetro el listado de registros en formato ASCII pasados como argumentro al programa en la línea de comando (records), así como el número de registros (nr_records), y devuelve la representación binaria en memoria de los mismos. Esta representación será un array de estructuras cuya memoria ha de reservarse con malloc() dentro de la propia función.
student_t* parse_records(char* records[], int nr_records) {
	// DONE
	student_t* res = malloc(sizeof(student_t)*nr_records);
	char* field;
	for(int i = 0; i < nr_records; i++){
		if (records[i] == NULL) err(6, "Incorrect record format `%s`. Expected student_id", field);
		field = strsep(records+i, ":");
		res[i].student_id = strtol(field,NULL,/*BASE*/10);

		if (records[i] == NULL) err(7, "Incorrect record format `%s`. Expected NIF", field);
		field = strsep(records+i, ":");
		strncpy(res[i].NIF, field, MAX_CHARS_NIF+1);

		if (records[i] == NULL) err(8, "Incorrect record format `%s`. Expected first_name", field);
		field = strsep(records+i, ":");
		res[i].first_name = malloc(strlen(field)*sizeof(char));
		strcpy(res[i].first_name,field);

		if (records[i] == NULL) err(9, "Incorrect record format `%s`. Expected last_name", field);
		field = strsep(records+i, ":");
		res[i].last_name = malloc(strlen(field)*sizeof(char));
		strcpy(res[i].last_name,field);
	}
	return res;
}
int dump_entries(student_t* entries, int nr_entries, FILE* students){
	// DONE
	int nr_errors = 0;
	int items_written;
	for(int i = 0; i < nr_entries; i++){
		// Write student_id
		items_written = fwrite(&entries[i].student_id,sizeof(int),1,students);
		nr_errors += items_written < 1;
		
		// Write NIF
		items_written = fwrite(&entries[i].NIF,sizeof(char),MAX_CHARS_NIF+1,students);
		nr_errors += items_written < MAX_CHARS_NIF+1;
		/* OPTMIZE_DATABASE_FILESIZE
		nr_errors += fputs(entries[i].NIF,students) == EOF;
		nr_errors += fputc('\0',students) == EOF;
		*/
		
		// Write first_name
		nr_errors += fputs(entries[i].first_name,students) == EOF;
		nr_errors += fputc('\0',students) == EOF;

		// Write last_name
		nr_errors += fputs(entries[i].last_name,students)  == EOF;
		nr_errors += fputc('\0',students) == EOF;
	}
	return nr_errors;
}
char* loadstr(FILE* fd){
	long n = 1; // We'll at least read one char to check if it's \0
	fpos_t checkpoint;

	if(fgetpos(fd,&checkpoint) != 0) err(5, "Couldn't get position from file");
	while(fgetc(fd) != '\0') n++;
	fsetpos(fd,&checkpoint);

	char* str = malloc(sizeof(char)*n);
	str = fgets(str,n,fd);
	char c;
	if ((c = fgetc(fd)) != '\0') 
		err(12, "Tried to get rid of ending \\0 but %c was there instead",c);
	return str;
}
student_t* read_student_file(FILE* students, int* nr_entries/*OUT*/){
	int error = 0;

	// Read the number of students in the file
	fread(nr_entries, sizeof(int), 1, students);
	student_t* res = malloc(sizeof(student_t)* (*nr_entries));

	// Read each student record
	for(int i = 0; i < *nr_entries; i++){
		error += fread(&res[i].student_id,sizeof(int),1,students) != 1;

		error += fread(&res[i].NIF, sizeof(char),MAX_CHARS_NIF+1,students) != MAX_CHARS_NIF+1;
		/* OPTMIZE_DATABASE_FILESIZE
		res[i].NIF = loadstr(students);
		*/

		if (error) err(10, "Corrupted file");

		res[i].first_name = loadstr(students);

		res[i].last_name  = loadstr(students);
	}
	return res;
}

void free_students(student_t* students, int nr_students){
	for(int i = 0; i < nr_students; i++){
		free(students[i].first_name);	
		free(students[i].last_name);	
	}
	free(students);
}
void add_number_of_records(FILE* file, int nr_new_records){
	int nr_records;
	fread (&nr_records, sizeof(int),1,file);
	nr_records += nr_new_records;
	rewind(file);
	fwrite(&nr_records, sizeof(int),1,file);
}
void write_students_to_file(char* filename, char opt, int nr_entries, student_t* students){
	if (opt != 'c' && opt != 'a') return;
	
	FILE* file = fopen(filename, opt=='c'?"w+":"r+");
	if(file == NULL) err(12, "Couldn't open database file `%s`", filename);

	if (opt == 'a'){ 
		add_number_of_records(file, nr_entries);
		fseek(file,0L,SEEK_END);
	}
	else fwrite(&nr_entries, sizeof(int),1,file);

	dump_entries(students, nr_entries, file);

	fflush(file);
	fclose(file);
}
student_t* read_students_from_file(char* filename, int* nr_entries/*OUT*/){
	FILE* file = fopen(filename, "r");
	student_t* students = read_student_file(file,nr_entries/*OUT*/);
	fflush(file);
	fclose(file);
	return students;
}
void print_entry(int num, student_t s){
	printf(entry_fmt, num, s.student_id, s.NIF, s.first_name, s.last_name);
}

int main(int argc, char** argv){
	exec_name = argv[0];
	if (argc < 3) {
		usage(1);
		err(1,"Not enough arguments");
	}

	// DONE: Consider not opening the file until you actually need to read
	//       that way you don't leave streams open
	//       "Make sure that when you use a file, only file operations can
	//       fail, and not anything else"
	char* filename = NULL;
	if (getopt(argc,argv,"f:") == 'f'){
		filename = optarg; // Creates the fine if it doesn't exist
	} else {
		usage(1);
		err(11, "You must provide a file argument at every execution");
	}

	char opt = getopt(argc,argv,"hlcaq");
	student_t* students;
	student_t* new_students;
	switch(opt){
		case 'h':{ // Help
			usage(0);
			break;
		}
		case 'l':{// List
			int nr_entries;
			students = read_students_from_file(filename,&nr_entries);
			for(int i = 0; i < nr_entries; i++)
				print_entry(i,students[i]);
			free_students(students,nr_entries);
			break;
		}
		case 'c': // Create
		case 'a': {// Append
			// Preparing for function calls
			int nr_new_students = argc - optind;

			// Actual logic
			new_students = parse_records(argv+optind, nr_new_students);
			int nr_students;
			if(opt == 'a'){
				// Ensure that we're not adding any entry with the same student_id as some other
				students = read_students_from_file(filename,&nr_students);
				for(int i = 0; i < nr_students; i++)
					for(int j = 0; j < nr_new_students; j++)
						if(new_students[j].student_id == students[i].student_id){
							fprintf(stderr,"Found duplicate student_id %i\n",students[i].student_id);
							return 4;
						}
				free_students(students,nr_students);
			}
			
			write_students_to_file(filename,opt,nr_new_students,new_students);

			// Cleanup
			free_students(new_students,nr_new_students);
			printf("%i %srecords written successfully\n",nr_new_students,opt=='a'?"extra ":"");
			break;
		}
		case 'q':{// Query
			char opt2 /*= getopt(argc-optind,argv+optind, "i:n:")*/;
			opt2=argv[optind][1];
			switch(opt2){
				case 'i': // ID
				case 'n':;// NIF
					// DONE
					int nr_entries;
					new_students = read_students_from_file(filename, &nr_entries /*OUT*/);
					int found_one_entry = 0;
					for(int i = 0; i < nr_entries; i++){
						if(( opt2=='i' && new_students[i].student_id == atoi(argv[optind+1]))
						|| ( opt2=='n' && strcmp(new_students[i].NIF,argv[optind+1])         )){
							print_entry(i,new_students[i]);
							found_one_entry = 1;
						}
					}
					if (!found_one_entry)
						printf("No entry was found\n");
					
					// Cleanup
					free_students(new_students,nr_entries);
					break;
				default:
					err(3, "Expected `-i` or `-n` after `-q`, received `%c`", optopt);
					break;
			}
			break;
		}
		default:
			usage(1);
			err(2,"Option not recognized `%c`", optopt);
	}
	return 0;
}
