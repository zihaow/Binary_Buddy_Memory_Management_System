CCFLAGS = -g -Wall

bb: test.o bb.o bb.h
	gcc $(CCFLAGS) -o bb test.c bb.o -lm

bb.o: bb.c bb.h
	gcc $(CCFLAGS) -c bb.c -lm 

clean:
	-rm bb bb.o test.o
