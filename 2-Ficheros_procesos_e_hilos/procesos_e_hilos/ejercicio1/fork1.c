#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){

	if(argc <= 1) {
		fprintf(stderr, "You need to give an executable name");
		exit(1);
	}

	int pid = fork();
	printf("I'm the %s, with pid %i and ppid %i\n",
		pid?"parent":"child",
		getpid(),
		getppid()
		);
	pid = fork();
	printf("I'm the %s, with pid %i and ppid %i\n",
		pid?"parent":"child",
		getpid(),
		getppid()
		);
	if(pid == 0)
		execvp(argv[1],argv+1);
	wait(NULL);
	return 0;
}
