#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t child_pid;
void main_signal_handler(int signal){
	if(signal == SIGALRM) kill(child_pid,SIGKILL);
}
/* void main_signal_handler(int signal, siginfo_t * info, void* ucontext){ */
/* 	if(signal == SIGALRM) kill(child_pid,SIGKILL); */
/* } */
int main(int argc, char** argv){
	if(argc <= 1) {
		fprintf(stderr, "Give me the name of an executable\n");
		exit(1);
	}
	// DONE: Crear un hijo
	child_pid = fork();
	if(child_pid == 0) execvp(argv[1],argv+1);

	// TODO: Terminar con la ejecución del hijo al recibir la señal
	struct sigaction action;
	/* action.sa_flags = SA_SIGINFO; */
	/* action.sa_sigaction = main_signal_handler; */
	action.sa_handler = main_signal_handler;
	sigaction(SIGALRM, &action, NULL);

	// TODO: Esperar a una señal de un temporizador
	alarm(5);
	int wstatus;
	wait(&wstatus);
	if(WIFEXITED(wstatus))
		printf("[N] El proceso terminó de forma normal\n");
	else 
		printf("[S] El proceso se termino por una señal\n");

	return 0;
}
