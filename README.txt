CMPUT 379 Assignment 1
Blaz Pocrnja
Shawn Sydia

How to Run:
	
	-"make mod<DRIVER NUMBER>" to compile executable.
	-"./mod<DRIVER NUMBER>" to run exectuable.

	*Where "<DRIVER NUMBER>" refers to 1,2,3 depending on the mem_mod version you want.
	
	-"make clean" to delete .o files.

Note:
In our output - 0 = R/W (Read/Write)
		1 = RO  (Read-Only)
		2 = NO  (No Access)
			

MEM_MOD1
	Allocates a block of PAGE_SIZE bytes of memory on the heap. The heap has a higher address region than the stack
        (which we manipulate in mem_mod2), and is read-writable.
	
MEM_MOD2
	Uses sbrk() to change the location of the program break, which defines the end of a process' data segment, 
	by incrementing the break by a PAGE_SIZE of bytes. This difference is lower in address space than the difference in MEM_MOD1, 
	but is similar in that the difference mem-region is read-writable as well.
	
MEM_MOD3
	Uses mprotect() to change the access mode of two regions of memory. Specifically, at one page the access mode is changed to read-only 
	for 1024 bytes (our program only reads access at the beginning of each page), and then no access for the following page for 1024 bytes.
	
	