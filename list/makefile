test : test.o SimpleL_pointer.o
	gcc -o test test.o SimpleL_pointer.o

test.o : test.c
	gcc -c -Wall test.c

SimpleL_pointer.o : SimpleL_pointer.c
	gcc -c -Wall SimpleL_pointer.c

clean :
	rm -f *.o test
