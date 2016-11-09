test : test.o buffer.o
	gcc -o test test.o buffer.o

test.o : test.c
	gcc -c -Wall test.c

buffer.o : buffer.c
	gcc -c -Wall buffer.c

clean :
	rm -f *.o test
