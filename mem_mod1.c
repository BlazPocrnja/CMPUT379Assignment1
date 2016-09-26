#include "memlayout.h"
#include <signal.h>
#include <stdio.h>

void sigsegv_handler(int signo) {
	
	printf("SEGFAULT\n");
	siglongjmp(env,1);
}

int main(void){

	//Setting up signal handler for SIGSEGV
	struct sigaction sigsegv_act;
	sigsegv_act.sa_flags = 0;
	sigsegv_act.sa_handler = sigsegv_handler;
	sigemptyset(&sigsegv_act.sa_mask);
	sigaction(SIGSEGV, &sigsegv_act, NULL);

	struct memregion regions[100]; 
	int arraySize = sizeof(regions)/sizeof(regions[0]);
	int actualSize;
	actualSize = get_mem_layout(&regions[0], arraySize);

	int i;
	for(i = 0; i < arraySize; ++i){
		printf("%x - %x %d \n", (unsigned int)regions[i].from, (unsigned int)regions[i].to, regions[i].mode);
	}
	

	
	
	return 0;
}