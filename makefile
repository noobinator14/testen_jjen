all : main.o buffer.o index.o mybfs.o SimpleL_pointer.o
	gcc -o path main.o buffer.o index.o mybfs.o SimpleL_pointer.o

main.o : main.c
	gcc -c -Wall main.c

buffer.o : buffer.c
	gcc -c -Wall buffer.c

index.o : index.c
	gcc -c -Wall index.c

mybfs.o : mybfs.c
	gcc -c -Wall mybfs.c

SimpleL_pointer.o : SimpleL_pointer.c
	gcc -c -Wall SimpleL_pointer.c

clean :
	rm -f *.o path


