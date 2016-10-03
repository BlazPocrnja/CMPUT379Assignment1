#include "memlayout.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int PAGE_SIZE = 4096;
jmp_buf env;

void sigsegv_handler(int signo) {

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
	struct memregion diffs[100];
	int diffSize = sizeof(diffs)/sizeof(diffs[0]);
	int arraySize = sizeof(regions)/sizeof(regions[0]);
	int actualSize;
	int actualDiffSize;
	int i;

    	//Allocate buffer with read and write
	//Source: http://www.tutorialspoint.com/unix_system_calls/mprotect.htm
    	char *p;
	p = malloc(1024+PAGE_SIZE-1);

	actualSize = get_mem_layout(&regions[0], arraySize);

	//Print memory layout
	printf("|******************************|\n");
	printf("MEMORY LAYOUT:");

	for(i = 0; i < actualSize; ++i){
		printf("\n%-10p - %-10p %d \n", regions[i].from, regions[i].to, regions[i].mode);
	}

	printf("\nMEMORY CONTAINS %d MEMORY REGIONS\n",actualSize);
	printf("|------------------------------|\n");

	//Align to a multiple of PAGE_SIZE, assumed to be a power of two 
    	p = (char *)(((int) p + PAGE_SIZE-1) & ~(PAGE_SIZE-1));
	
	//Mark buffer READ-ONLY 
	mprotect(p, 1024, PROT_READ);
	//Mark one PAGE_SIZE further NO-ACCESS
	mprotect(p+PAGE_SIZE, 1024, PROT_NONE);


 	//Find difference after change
	actualDiffSize = get_mem_diff(regions, arraySize, diffs, diffSize);

    	printf("\n|******************************|\n");
    	printf("DIFFERENCE: %d MEMORY REGIONS\n",actualDiffSize);
	for(i = 0; i < actualDiffSize; ++i){
		printf("\n%-10p - %-10p %d \n", diffs[i].from, diffs[i].to, diffs[i].mode);
	}
	printf("|------------------------------|\n");

	//Print new memory layout
	actualSize = get_mem_layout(&regions[0], arraySize);
    	printf("\n|******************************|\n");
    	printf("NEW MEMORY LAYOUT:");
	for(i = 0; i < actualSize; ++i){
		printf("\n%-10p - %-10p %d \n", regions[i].from, regions[i].to, regions[i].mode);
	}

	printf("\nMEMORY CONTAINS %d MEMORY REGIONS\n",actualSize);
	printf("|------------------------------|\n");





	return 0;
}
























