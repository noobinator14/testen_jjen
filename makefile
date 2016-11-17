bydir : main.o buffer.o index.o bydir.o SimpleL_pointer.o hash.o
	gcc -o path main.o buffer.o index.o bydir.o SimpleL_pointer.o hash.o

main.o : main.c
	gcc -c -Wall main.c

buffer.o : buffer/buffer.c
	gcc -c -Wall buffer/buffer.c

index.o : index/index.c
	gcc -c -Wall index/index.c

bydir.o : Bidirectional_BFS/bydir.c
	gcc -c -Wall Bidirectional_BFS/bydir.c

SimpleL_pointer.o : list/SimpleL_pointer.c
	gcc -c -Wall list/SimpleL_pointer.c

hash.o : hashtable/hash.c
	gcc -c -Wall hashtable/hash.c

clean :
	rm -f *.o path
