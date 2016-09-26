#include "memlayout.h"

int get_mem_layout (struct memregion *regions, unsigned int size){
	
	int actualSize = 0;
	unsigned int i = 0; 
	int jmpVar = 0;
	char *address = (char*)0;
	char data;

	enum tryMode{
		reading,
		writing	
	};

	enum tryMode tryingMode = reading;
	unsigned char lastPageMode, currentPageMode;
	
	//Determine accessibility of location 0
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
		
	}
	else{
		//Something failed

		if(tryingMode == writing){
			//It failed trying to write but suceeded reading
			currentPageMode = MEM_RO;

		}
		else{
			//It failed trying to read
			currentPageMode = MEM_NO;
		}

		
	}
	if(actualSize <= size){
		regions[0].from = 0;
		regions[0].mode = currentPageMode;
	}
	++actualSize;

	for(i = 1; i <= 0xffffffff/PAGE_SIZE; ++i){
		address = (char*)(long)(i*PAGE_SIZE);
		lastPageMode = currentPageMode;

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
			currentPageMode = MEM_RW;	
		}
		else{
			//Something failed

			if(tryingMode == writing){
				//It failed trying to write but suceeded reading
				currentPageMode = MEM_RO;
			}
			else{
				//It failed trying to read
				currentPageMode = MEM_NO;
			}
			
		}
		
		if(currentPageMode != lastPageMode){
			//If we're still within the array
			if(actualSize <= size){
				//The last page was the final page of the previous memregion
				regions[actualSize - 1].to = address - PAGE_SIZE;

				//Add data to the new region
				regions[actualSize].mode = currentPageMode;
				regions[actualSize].from = address;
			}
			++actualSize;	
		}		
	}
	//Final region ends at address 0xffffffff
	regions[actualSize-1].to = (char*)0xffffffff;
	return actualSize;
}

int get_mem_diff (struct memregion *regions, unsigned int howmany,
struct memregion *thediff, unsigned int diffsize){
	return 0;
}
