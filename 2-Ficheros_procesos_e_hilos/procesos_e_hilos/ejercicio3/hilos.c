#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int num_usuario;
	char prioridad;
} args_t;

void* do_stuff(void* args){
	int num_usuario = ((args_t*)args)->num_usuario;
	char prioridad = ((args_t*)args)->prioridad;
	free(args);
	printf("Hilo de usuario número %i (%c), con id %lu\n",
		num_usuario,
		prioridad,
		pthread_self()
	);
	return NULL;
}

int main(int argc, char** argv){
	pthread_t* tids = malloc((argc)*sizeof(pthread_t));
	for(int i = 1; i < argc; i++){
		// Creando hilo para usuario i
		args_t* args = malloc(sizeof(args_t));
		args->num_usuario = i;
		args->prioridad = i%2?'P':'N';
		pthread_create(&tids[i],NULL,do_stuff,(void*)args);
		/* printf("DEBUG: %lu\n", tids[i]); */
	}
	for(int i = 1; i < argc; i++)
		pthread_join(tids[i],NULL);
	
	return 0;
}
