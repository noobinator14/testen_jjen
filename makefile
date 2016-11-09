bfs : main_bfs.o buffer.o index.o mybfs.o SimpleL_pointer.o
	gcc -o path main_bfs.o buffer.o index.o mybfs.o SimpleL_pointer.o

bydir : main_bydir.o buffer.o index.o bydir.o SimpleL_pointer.o
	gcc -o path main_bydir.o buffer.o index.o bydir.o SimpleL_pointer.o

main_bfs.o : main_bfs.c
	gcc -c -Wall main_bfs.c

main_bydir.o : main_bydir.c
	gcc -c -Wall main_bydir.c

buffer.o : buffer/buffer.c
	gcc -c -Wall buffer/buffer.c

index.o : index/index.c
	gcc -c -Wall index/index.c

mybfs.o : BFS/mybfs.c
	gcc -c -Wall BFS/mybfs.c

bydir.o : Bidirectional_BFS/bydir.c
	gcc -c -Wall Bidirectional_BFS/bydir.c

SimpleL_pointer.o : list/SimpleL_pointer.c
	gcc -c -Wall list/SimpleL_pointer.c

clean :
	rm -f *.o path
