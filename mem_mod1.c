#include "memlayout.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

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

	actualSize = get_mem_layout(&regions[0], arraySize);
	
	for(i = 0; i < actualSize; ++i){
		printf("\n%-10p - %-10p %d \n", regions[i].from, regions[i].to, regions[i].mode);
	}

	printf("\n%d\n",actualSize);

	int *test;
	test = (int*) calloc(1000, sizeof(int));
	
	actualDiffSize = get_mem_diff(regions, arraySize, diffs, diffSize);

	for(i = 0; i < actualDiffSize; ++i){
		printf("\n%-10p - %-10p %d \n", diffs[i].from, diffs[i].to, diffs[i].mode);
	}

	printf("\n%d\n",actualDiffSize);

	actualSize = get_mem_layout(&regions[0], arraySize);
	
	for(i = 0; i < actualSize; ++i){
		printf("\n%-10p - %-10p %d \n", regions[i].from, regions[i].to, regions[i].mode);
	}

	printf("\n%d\n",actualSize);

	
	
	return 0;
}