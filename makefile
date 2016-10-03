CC=gcc
CFLAGS=-m32
DEPS = memlayout.h
OBJ1 = memlayout.o mem_mod1.o 
OBJ2 = memlayout.o mem_mod2.o 
OBJ3 = memlayout.o mem_mod3.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mod1: $(OBJ1)
	gcc -o $@ $^ $(CFLAGS)

mod2: $(OBJ2)
	gcc -o $@ $^ $(CFLAGS)

mod3: $(OBJ3)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o mod1 mod2 mod3
