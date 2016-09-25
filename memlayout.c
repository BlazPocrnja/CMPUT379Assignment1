#include "memlayout.h"
#include <setjmp.h>

#define PAGE_SIZE 4096

struct memregion {
	void *from;
	void *to;
	unsigned char mode; /* MEM_RW, or MEM_RO, or MEM_NO */
};

int get_mem_layout (struct memregion *regions, unsigned int size){
	
	int actualSize = 0;
	int i = 0; 
	int j = 0;
	int jmpVar = 0;
	jmp_buf env;
	char *address = 0;
	char data;

	enum tryMode{
		reading,
		writing	
	};

	enum tryMode tryingMode = reading;
	unsigned char startingPageMode, currentPageMode;
	
	for(i = 0; i <= 0xffffffff-PAGE_SIZE; i += PAGE_SIZE){
		
		++actualSize;
		address = (char*)i;

		if(actualSize <= size){
			/*Assign a starting address for the current memregion*/
			regions->from = address;
		}

		/*Determine accessibility of the region*/
		jmpVar = sigsetjmp(env, 1);
		if(jmpVar = 0){	
			/*Check if we can read*/
			tryingMode = reading;
			data = *address;

			/*Check if we can write*/
			tryingMode = writing;
			*address = data;

			/*Writing successful*/
			startingPageMode = MEM_RW;
			
		}
		else{
			/*Something failed*/

			if(tryingMode == writing){
				/*It failed trying to write but suceeded reading*/
				startingPageMode = MEM_RO;
			}
			else{
				/*It failed trying to read*/
				startingPageMode = MEM_NO;
			}
			
		}
		
		if(actualSize <= size){
			regions->mode = startingPageMode;
		}

		for(j = i + PAGE_SIZE; j <= 0xffffffff-PAGE_SIZE; j += PAGE_SIZE){

			address = (char*)j;

			/*Determine accessibility of the page*/
			jmpVar = sigsetjmp(env, 1);
			if(jmpVar = 0){	
				/*Check if we can read*/
				tryingMode = reading;
				data = *address;

				/*Check if we can write*/
				tryingMode = writing;
				*address = data;

				/*Writing successful*/
				currentPageMode = MEM_RW;
			
			}
			else{
				/*Something failed*/

				if(tryingMode == writing){
					/*It failed trying to write but suceeded reading*/
					currentPageMode = MEM_RO;
				}
				else{
					/*It failed trying to read*/
					currentPageMode = MEM_NO;
				}
			
			}

			/*Check if start of new memregion*/
			if(startingPageMode != currentPageMode){
				
				/*If we're still within the array*/
				if(actualSize <= size){
					/*The last page was the final location of the previous memregion*/
					regions->to = address - PAGE_SIZE;
					//Increment array of memregions
					regions += sizeof(struct memregion);
				}

				/*Move i to j's location*/
				i = j;

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
