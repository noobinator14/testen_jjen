mygod : test_main.o buffer.o index.o bydir_SCC.o SimpleL_pointer.o hash.o scc.o connected_comp.o mybfs.o bydir.o grail.o bidir_grail.o queue.o job_sched.o
	gcc -o path -pthread test_main.o buffer.o index.o bydir_SCC.o SimpleL_pointer.o hash.o scc.o connected_comp.o mybfs.o bydir.o grail.o bidir_grail.o queue.o job_sched.o

test_main.o : test_main.c
	gcc -c -Wall test_main.c

buffer.o : buffer/buffer.c
	gcc -c -Wall buffer/buffer.c

index.o : index/index.c
	gcc -c -Wall index/index.c

bydir_SCC.o : Bidirectional_BFS_SCC/bydir_SCC.c
	gcc -c -Wall Bidirectional_BFS_SCC/bydir_SCC.c

bydir.o : Bidirectional_BFS/bydir.c
	gcc -c -Wall Bidirectional_BFS/bydir.c

SimpleL_pointer.o : list/SimpleL_pointer.c
	gcc -c -Wall list/SimpleL_pointer.c

hash.o : hashtable/hash.c
	gcc -c -Wall hashtable/hash.c

connected_comp.o : connected_comp/connected_comp.c
	gcc -c -Wall connected_comp/connected_comp.c

mybfs.o : BFS/mybfs.c
	gcc -c -Wall BFS/mybfs.c

scc.o : strongly_connected_comp/scc.c
	gcc -c -Wall strongly_connected_comp/scc.c

grail.o : grail/grail.c
	gcc -c -Wall grail/grail.c

bidir_grail.o : Bidirectional_BFS_Grail/bidir_grail.c
	gcc -c -Wall Bidirectional_BFS_Grail/bidir_grail.c

queue.o : queue/queue.c
	gcc -c -Wall queue/queue.c

job_sched.o : job_sched/job_sched.c
	gcc -c -Wall job_sched/job_sched.c

clean :
	rm -f *.o path
