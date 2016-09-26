#include "memlayout.h"
#include <stdio.h>

#define PAGE_SIZE 4096

int get_mem_layout (struct memregion *regions, unsigned int size){
	
	int actualSize = 0;
	unsigned int i = 0; 
	unsigned int j = 0;
	int jmpVar = 0;
	char *address = 0;
	char data;

	enum tryMode{
		reading,
		writing	
	};

	enum tryMode tryingMode = reading;
	unsigned char startingPageMode, currentPageMode;
	
	for(i = 0; i <= 0xffffffff/PAGE_SIZE; ++i){

		++actualSize;
		address = (char*)(i*PAGE_SIZE);

		if(actualSize <= size){
			//Assign a starting address for the current memregion
			regions->from = address;
		}

		//Determine accessibility of the region
		jmpVar = sigsetjmp(env, 1);
		if(jmpVar == 0){	
			//Check if we can read
			tryingMode = reading;
			data = *address;
			
			//Check if we can write
			tryingMode = writing;
			*address = data;

			//Writing successful
			startingPageMode = MEM_RW;
			printf("Writing");
			
		}
		else{
			//Something failed

			if(tryingMode == writing){
				//It failed trying to write but suceeded reading
				startingPageMode = MEM_RO;
				printf("Read only");
			}
			else{
				//It failed trying to read
				startingPageMode = MEM_NO;
				printf("no");
			}
			
		}
		
		if(actualSize <= size){
			regions->mode = startingPageMode;
		}
		for(j = i + 1; j <= 0xfffffff/PAGE_SIZE; ++j){

			address = (char*)(j*PAGE_SIZE);
			
			//Determine accessibility of the page
			jmpVar = sigsetjmp(env, 1);
			if(jmpVar == 0){	
				//Check if we can read
				tryingMode = reading;
				data = *address;

				//Check if we can write
				tryingMode = writing;
				*address = data;

				//Writing successful
				currentPageMode = MEM_RW;
				printf("Writing");
			
			}
			else{
				//Something failed

				if(tryingMode == writing){
					//It failed trying to write but suceeded reading
					currentPageMode = MEM_RO;
					printf("Read only");
				}
				else{
					//It failed trying to read
					currentPageMode = MEM_NO;
					printf("no\n");
				}
			
			}

			//Check if start of new memregion
			if(startingPageMode != currentPageMode){
				
				//If we're still within the array
				if(actualSize <= size){
					//The last page was the final location of the previous memregion
					regions->to = address - PAGE_SIZE;
					//Increment array of memregions
					regions += sizeof(struct memregion);
				}

				//Move i to j-1 location, 1 will be added in the next iteration
				i = j - 1;

				break;
			}
		}
		
	}

	return actualSize;
}

int get_mem_diff (struct memregion *regions, unsigned int howmany,
struct memregion *thediff, unsigned int diffsize){
	return 0;
}
