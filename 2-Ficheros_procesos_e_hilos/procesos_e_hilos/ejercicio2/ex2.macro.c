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

void do_exec(int i){
	char process_name[2];
	sprintf(process_name,"%i",i);
	if(i % 2) // Impar 
		execlp("echo","echo","I'm process",process_name,NULL);
	else
		execl("/usr/bin/echo","/usr/bin/echo","I'm process", process_name,NULL);
}

#define process(i) int process_##i = fork(); if(process_##i == 0)
/* #define wait_for(pid) waitpid(pid,NULL,0) */
void wait_for(int pid){ waitpid(pid, NULL, 0); }

int main(int argc, char** argv){
	process(1){
		do_exec(1);
	}
	process(2){
		wait_for(process_1);
		do_exec(2);
	}
	process(3){
		wait_for(process_2);
		do_exec(3);
	}
	process(4){
		wait_for(process_2);
		do_exec(4);
	}
	process(5){
		wait_for(process_1);
		do_exec(5);
	}
	process(6){
		wait_for(process_4);
		wait_for(process_5);
		do_exec(6);
	}
	process(7){
		wait_for(process_1);
		do_exec(7);
	}
	process(8){
		wait_for(process_3);
		wait_for(process_6);
		wait_for(process_7);
		do_exec(8);
	}
	return 0;
}
