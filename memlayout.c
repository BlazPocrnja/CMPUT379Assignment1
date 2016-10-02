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
				//The last address location was the final location of the previous memregion
				regions[actualSize - 1].to = address - 1;

				//Add data to the new region
				regions[actualSize].mode = currentPageMode;
				regions[actualSize].from = address;
			}
			++actualSize;	
		}		
	}
	//Final region ends at address 0xffffffff
	address = (char*)(long)(i*PAGE_SIZE);
	regions[actualSize-1].to = address-1;
	return actualSize;
}

int get_mem_diff (struct memregion *regions, unsigned int howmany,
struct memregion *thediff, unsigned int diffsize){
		
	int actualDiffSize = 0;
	unsigned int i = 0; 
	unsigned int j = 0;
	int jmpVar = 0;
	char *address = (char*)0;
	char data;

	enum tryMode{
		reading,
		writing	
	};

	enum tryMode tryingMode = reading;
	unsigned char oldCurrentPageMode, oldLastPageMode, currentPageMode, lastPageMode;

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
	
	oldCurrentPageMode = regions[0].mode;
	
	if(currentPageMode != oldCurrentPageMode){
		if(actualDiffSize <= diffsize){
			thediff[0].from = 0;
			thediff[0].mode = currentPageMode;
		}
		++actualDiffSize;
	}


	for(i = 1; i <= 0xffffffff/PAGE_SIZE; ++i){
		address = (char*)(long)(i*PAGE_SIZE);
		lastPageMode = currentPageMode;
		oldLastPageMode = oldCurrentPageMode;

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
		
		//Get mode from old region you're in by checking all .to addresses
		for(j = 0; j < howmany; ++j){
			if(address <= (char*)regions[j].to){
				oldCurrentPageMode = regions[j].mode;
				break;
			}
		}
		
		//Checks all modes new and old and decides whether to add a starting address or an ending address to a diff region
		if(lastPageMode != currentPageMode){		
			if(lastPageMode != oldLastPageMode){
				if(actualDiffSize <= diffsize){
					thediff[actualDiffSize-1].to = address-1;
				}	
			}
			if(currentPageMode != oldCurrentPageMode){
				if(actualDiffSize <= diffsize){
					thediff[actualDiffSize].from = address;
					thediff[actualDiffSize].mode = currentPageMode;
				}
				++actualDiffSize;
			}
		}
	}

	//Set "to" address for the last diff region
	thediff[actualDiffSize-1].to = thediff[actualDiffSize-1].from + PAGE_SIZE;

	
	return actualDiffSize;

}
