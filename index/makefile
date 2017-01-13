test : test.o index.o
	gcc -o test test.o index.o

test.o : test.c
	gcc -c -Wall test.c

index.o : index.c
	gcc -c -Wall index.c

clean :
	rm -f *.o test
