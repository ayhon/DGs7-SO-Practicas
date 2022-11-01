#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
    ┌─►P3────────┐
    │            │
 ┌─►P2           │
 │  │            │
P1  └─►P4─┐      ▼
 │        ├►P6──►P8
 ├─►P5────┘      ▲
 │               │
 │               │
 └─►P7───────────┘

Process P6 cannot be started
before P4 and P5 finish

*/

/* #define process(i) int process_##i=fork();if(process_##i==0)do_exec(i) */
// TO UNDO :%s/process(\(\d\))/int process_\1 = fork()\r\tif(process_\1 == 0)/g 
/* #define wait_for(pid) waitpid(pid,NULL,0) */
int process(int i){
	int res = fork();
	if(res == 0){
		char process_name[2] = {i+'0','\0'};
		if(i % 2 == 1) execlp("echo","echo","I'm process",process_name,NULL);
		else execl("/usr/bin/echo","/usr/bin/echo","I'm process", process_name,NULL);
	}
	return res;
}
void wait_for(int pid){ waitpid(pid, NULL, 0); }

int main(int argc, char** argv){
	int process_1 = process(1);

	wait_for(process_1);
	int process_2 = process(2);
	int process_5 = process(5);
	int process_7 = process(7);

	wait_for(process_2);
	int process_3 = process(3);
	int process_4 = process(4);

	wait_for(process_4);
	wait_for(process_5);
	int process_6 = process(6);

	wait_for(process_3);
	wait_for(process_6);
	wait_for(process_7);
	process(8);

	return 0;
}
