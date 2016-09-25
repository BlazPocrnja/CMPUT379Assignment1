#ifndef memlayout_h
#define memlayout_h

#define MEM_RW 0
#define MEM_RO 1
#define MEM_NO 2

struct memregion;

int get_mem_layout (struct memregion *regions, unsigned int size);

int get_mem_diff (struct memregion *regions, unsigned int howmany,
struct memregion *thediff, unsigned int diffsize);
#endif