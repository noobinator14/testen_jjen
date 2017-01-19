#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "./index/index.h"
#include "./buffer/buffer.h"
#include "./Bidirectional_BFS/bydir.h"
#include "./list/Pointer_List.h"
#include "./hashtable/hash.h"
#include "./connected_comp/connected_comp.h"
#include "./Bidirectional_BFS_SCC/bydir_SCC.h"
#include "./strongly_connected_comp/scc.h"
#include "./grail/grail.h"
#include "./Bidirectional_BFS_Grail/bidir_grail.h"
#include "./job_sched/job_sched.h"

#define TALKTOME 1		// 0: print only results , 1: print messages too
#define THREAD_POOL_SIZE 2

long GLOBAL_VISIT, GLOBAL_GRAIL_VISIT, *visited_grail, *visited;
NodeIndex *out_index, *in_index;
Buffer *out_buffer, *in_buffer;
bucketPtr *hash_index;
int current_out_buf_size, current_out_ind_size, current_in_buf_size, current_in_ind_size;
int scc_initiated, grail_initiated, cc_initiated, no_more_jobs, oura_is_empty, dyn, sta, queries_that_use_updateIndex, queries;
uint32_t bNode;
FILE *fp, *fp1, *fp2;
CC *components;
scc *SCC;
GrailIndex *grailIndex;
JobSched *scheduler;

/* Function for master thread */
void * thread_master(void *arg) {

	int i,err,count,u;
	char s[2],type[8];
	uint32_t from,to;
	Job *j;

	/* Diavasma kai twn 2 arxeiwn mesa se auto to while-loop */
	while (1) {
		if(fscanf(fp, "%1s", s) == EOF) {		//Reads 1 char
			if(u==0) {			// kaluptei th periptwsh pou to 1o arxeio den exei 'S' sto telos
				fp = fp2;
				count=0;
				if(fclose(fp1) != 0) {
					printf("FAIL: fclose(fp1) \n");
					exit(-1);
				}
				u++;
				//diabasma STATIC/DYNAMIC
				if(fscanf(fp, "%s", type) == EOF)
					break;
				if(strcmp(type, "DYNAMIC") == 0)
					dyn=1;
				else if(strcmp(type, "STATIC") == 0) {
					sta=1;
					if (TALKTOME>0)
						printf("Master, we are initiating SCC functions\n");
					SCC = estimatedStronglyConnectComp(out_buffer,in_buffer,out_index,in_index,
										current_out_ind_size,current_in_ind_size,bNode);
					if(SCC == NULL) {
						printf("FAIL: estimate SCC\n");
						exit(-1);
					}
					else {
						scc_initiated=1;
						if (TALKTOME>0)
							printf("Master, SCC is ready\n");
						getchar();
					}
				}
				else {
					if(fseek(fp, -7, SEEK_CUR) != 0) {	//Goes back at the start of the line
						perror("fseek");
						exit(-1);
					}		
				}
				continue;
			}
			else
				break;
		}
		count++;
		if(fp == fp1) {
			if(strcmp(s, "S") == 0) {
				fp = fp2;
				count=0;
				if(fclose(fp1) != 0) {
					perror("fclose");
					exit(-1);
				}
				u++;
				//diabasma STATIC/DYNAMIC
				if(fscanf(fp, "%s", type) == EOF)
					break;
				if(strcmp(type, "DYNAMIC") == 0)
					dyn=1;
				else if(strcmp(type, "STATIC") == 0) {
					sta=1;
					if (TALKTOME>0)
						printf("Master, we are initiating SCC functions\n");
					SCC=estimatedStronglyConnectComp(out_buffer,in_buffer,out_index,in_index,
										current_out_ind_size,current_in_ind_size,bNode);
					if(SCC == NULL) {
						printf("FAIL: estimate SCC\n");
						exit(-1);
					}
					else {
						scc_initiated=1;
						if (TALKTOME>0)
							printf("Master, SCC is ready\n");
						getchar();
					}
				}
				else {
					sta=1;		// STATIC by default
					SCC=estimatedStronglyConnectComp(out_buffer,in_buffer,out_index,in_index,
										current_out_ind_size,current_in_ind_size,bNode);
					if(SCC == NULL) {
						printf("FAIL: estimate SCC\n");
						exit(-1);
					}
					else {
						scc_initiated=1;
						if (TALKTOME>0)
							printf("Master, SCC is ready\n");
						getchar();
					}
					if(fseek(fp, -1, SEEK_CUR) != 0) {	//Goes back at the start of the line
						perror("fseek");
						exit(-1);
					}		
				}
				continue;
			}
			else {
				if(fseek(fp, -1, SEEK_CUR) != 0) {	//Goes back at the start of the line
					perror("fseek");
					exit(-1);
				}
				if(fscanf(fp, "%u %u", &from, &to) == EOF)	//Reads both nodes
					break;
			}
			//if(count % 100000 == 0)
				//printf("Count: %ld\n", count);
		}
		else if(fp == fp2) {
			if (count==1 && dyn==1) {			// initialize Connected Components
				if (TALKTOME>0)
					printf("Let's estimate CC\n");
				components=estimateConnectedComponents(out_index,in_index,out_buffer,in_buffer,
										current_out_ind_size,current_in_ind_size);
				if (components!=NULL) {
					if (TALKTOME>0)
						printf("Found %d cc...\n",components->next_CC);
					cc_initiated=1;
				}
				else
					exit(-1);
			}
			else if (count==1 && sta==1) {			// SCC has been initialized, so initialize Grail index
				grailIndex=buildGrailIndex(out_index,out_buffer,current_out_ind_size,SCC);
				if (grailIndex==NULL) {
					printf("FAIL: Initialize Grail index\n");
					exit(-1);
				}
				visited_grail=malloc(bNode*sizeof(long));
				if (visited_grail==NULL) {
					printf("FAIL: malloc visited_grail\n");
					exit(-1);
				}
				for (i=0;i<bNode;i++)
					visited_grail[i]=-1;
				GLOBAL_GRAIL_VISIT=0;
				grail_initiated=1;
			}
			if(strcmp(s, "F") == 0) {
				if (queries>0 && components!=NULL && dyn == 1) {
					(components->metric)=((double) queries_that_use_updateIndex / queries) ;
					if (TALKTOME>0) {
						printf("queries_that_use_updateIndex=%d\n",queries_that_use_updateIndex);
						printf("queries=%d\n",queries);
						printf("metric=%f\n",(components->metric));
					}
					if ( (components->metric) > 0.47) {
						if ((rebuildIndexes(components))==FAILURE)
							printf("FAIL: rebuildIndexes\n");
						else if (TALKTOME>0)
							printf("CCindex rebuilt!!!\n");
					}						
				}
				continue;
			}
			if(fscanf(fp, "%u %u", &from, &to) == EOF) {	//Reads both nodes
				break;
			}
			//if(count % 1000 == 0)
			//	printf("Count: %ld\n", count);
		}
		/* Create job */
		if (fp==fp1)
			j=create_job('A',from,to);
		else if (strcmp(s,"A")==0 || strcmp(s,"Q")==0)
			j=create_job(s[0],from,to);
		else {
			printf("Unknown job action: %s\n",s);
			exit(-1);
		}
		/* Submit job */
		err=pthread_mutex_lock(&(scheduler->oura_mtx));			// LOCK mutex for job queue
		if (err!=0) {
			fprintf(stderr, "pthread_mutex_lock (master thread): %s\n", strerror(err));
			exit(-1);
		}
		if ((submit_job(scheduler,j))!=0) {
			printf("FAIL: submit job\n");
			exit(-1);
		}
		oura_is_empty=0;
		err=pthread_mutex_unlock(&(scheduler->oura_mtx));		// UNLOCK mutex for job queue
		if (err!=0) {
			fprintf(stderr, "pthread_mutex_unlock (master thread): %s\n", strerror(err));
			exit(-1);
		}
		/* ksupnhma enos thread epeidh valame job*/
		pthread_cond_signal(&(scheduler->oura_has_job));
	}
	no_more_jobs=1;
	/* ksupnhma olwn twn thread gia na termatisoun th leitourgia tous */
	pthread_cond_broadcast(&(scheduler->oura_has_job));
	/* Join all worker threads */
	for (i=1;i<scheduler->execution_threads;i++) {
		err=pthread_join(scheduler->tids[i],NULL);
		if (err!=0) {
			fprintf(stderr, "pthread_join (worker thread): %s\n", strerror(err));
			exit(-1);
		}
	}
	/* Terminate master thread */
	pthread_exit(NULL);
}


/* Function for worker threads */
void * thread_worker(void *arg) {

	int i, err, visit_sze=0, old_visit_sze, hash_value, multiplier=1, ret;
	long s_path_fo, c, head, last, new, b, offset;
	uint32_t from, to;
	char mode;
	void *tmp, *tmp2;
	list_node nod, nod2;
	GRAIL_ANSWER ans;
	Job *j;

	while (1) {
		/* Acquire a job from queue */
		err=pthread_mutex_lock(&(scheduler->oura_mtx));			// LOCK mutex for job queue
		if (err!=0) {
			fprintf(stderr, "pthread_mutex_lock (worker thread): %s\n", strerror(err));
			exit(-1);
		}
		if (Oyra_keni(scheduler->oura) && no_more_jobs==1) {		// all jobs are done, wake up other workers & terminate
			pthread_cond_broadcast(&(scheduler->oura_has_job));
			pthread_exit(NULL);
		}
		while (oura_is_empty==1) {
			pthread_cond_wait(&(scheduler->oura_has_job),&(scheduler->oura_mtx));
			if (Oyra_keni(scheduler->oura) && no_more_jobs==1)		// all jobs are done, terminate thread
				pthread_exit(NULL);	
		}
		if (!Oyra_keni(scheduler->oura))
			j=Oyra_apomakrynsh(scheduler->oura);
		if (Oyra_keni(scheduler->oura))
			oura_is_empty=1;
		err=pthread_mutex_unlock(&(scheduler->oura_mtx));		// UNLOCK mutex for job queue
		if (err!=0) {
			fprintf(stderr, "pthread_mutex_unlock (worker thread): %s\n", strerror(err));
			exit(-1);
		}
		mode=j->mode;
		from=j->node1;
		to=j->node2;
		/* Execute job */
		if (mode=='Q') {
			if(dyn == 1) {
				// while (!cc_initiated) //nomizw de xreiazetai,prwta arxikopoiountai ta CC kai meta mpainei job sthn Q
				// 	pthread_cond_wait ???
				queries++;
				int predict=CC_search_for_connection(components,from,to,&queries_that_use_updateIndex);
				if (predict==0) {
					printf("-1\n");
					continue;
				}
				info_deikti next_nodes1=LIST_dimiourgia();
				info_deikti next_nodes2=LIST_dimiourgia();
				if (next_nodes1==NULL || next_nodes2==NULL)
					exit(-1);
				if (GLOBAL_VISIT==0) {
					if (current_out_ind_size > current_in_ind_size)
						visit_sze=current_out_ind_size;
					else
						visit_sze=current_in_ind_size;
					visited=malloc(visit_sze * sizeof(long));
					if (visited==NULL) {
						printf("FAIL: malloc visited\n");
						exit(-1);
					}
					for(i=0;i<visit_sze;i++)
						visited[i]=-1;
				}
				else {
					if ( (current_out_ind_size > visit_sze) || (current_in_ind_size > visit_sze) ) {
						old_visit_sze=visit_sze;
						if (current_out_ind_size > current_in_ind_size)
							visit_sze=current_out_ind_size;
						else
							visit_sze=current_in_ind_size;
						tmp2=realloc(visited,visit_sze * sizeof(long));
						if (tmp2!=NULL) {
							visited=tmp2;
							for(i=old_visit_sze; i<visit_sze; i++)
								visited[i]=-1;
						}
						else {
							printf("FAIL: realloc visited\n");
							exit(-1);
						}
					}
				}
				s_path_fo=mybfs(from, to, out_buffer, in_buffer, out_index, in_index, current_out_ind_size,
						current_in_ind_size, &next_nodes1, &next_nodes2, visited, visit_sze);
				if (s_path_fo==-2) {
					printf("Error inside bidirectional. Terminating.\n");
					exit(-1);
				}
				else
					printf("%ld\n",s_path_fo);
				GLOBAL_VISIT++;
				if (GLOBAL_VISIT==2147483647) {
					printf("GLOBAL_VISIT: Too many queries..!!\n");
					exit(-1);
				}
				LIST_katastrofi(&next_nodes1);
				LIST_katastrofi(&next_nodes2);
			}
			else if(sta == 1){
				// while (!scc_initiated || !grail_initiated) // paromoia logikh me pio panw
				// 	pthread_cond_wait
				ans=isReachableGrailIndex(grailIndex,SCC,from,to);
				if (ans==NO) {
					printf("-1\n");
					continue;
				}
				else if (ans==YES) {		// euresh suntomoterou monopatiou eswterika sto SCC
					//visited eksw
					info_deikti next_nodes1=LIST_dimiourgia();
					info_deikti next_nodes2=LIST_dimiourgia();
					if (next_nodes1==NULL || next_nodes2==NULL)
						exit(-1);
					s_path_fo = estimateShortestPathStronglyConnectedComponents(SCC, out_buffer,
						in_buffer, out_index, in_index, current_out_ind_size, current_in_ind_size,
						from, to, &next_nodes1, &next_nodes2);
					if (s_path_fo==-2) {
						printf("Error inside bidirectional. Terminating.\n");
						exit(-1);
					}
					else
						printf("%ld\n",s_path_fo);
					LIST_katastrofi(&next_nodes1);
					LIST_katastrofi(&next_nodes2);
				}
				else if (ans==MAYBE) {
					info_deikti next_nodes1=LIST_dimiourgia();
					info_deikti next_nodes2=LIST_dimiourgia();
					if (next_nodes1==NULL || next_nodes2==NULL)
						exit(-1);
					s_path_fo=bidir_grail(from,to,out_buffer,in_buffer,out_index,in_index,
							current_out_ind_size,current_in_ind_size,&next_nodes1,&next_nodes2,
							visited_grail,bNode+1,grailIndex,SCC);
					if (s_path_fo==-2) {
						printf("Error inside bidir_grail. Terminating.\n");
						exit(-1);
					}
					else
						printf("%ld\n",s_path_fo);
					GLOBAL_GRAIL_VISIT++;
					if (GLOBAL_GRAIL_VISIT==2147483647) {
						printf("GLOBAL_GRAIL_VISIT: Too many queries..!!\n");
						exit(-1);
					}
					LIST_katastrofi(&next_nodes1);
					LIST_katastrofi(&next_nodes2);
				}
			}
		}
		else if (mode=='A') {
			if (from>=0 && to>=0 && from!=to) {
				/* Biggest node number */
				if(from > bNode)
					bNode = from;
				if(to > bNode)
					bNode = to;
				/* Check hash if neighbour exists */
				if (from<current_out_ind_size) {
					hash_value=hash_func(to);
					if (hash_index[from]==NULL) {
						hash_index[from]=create_hashtable();
						if (hash_index[from]==NULL)
							exit(-1);
						c=check_add(&(hash_index[from][hash_value]),to,1);
					}
					else
						c=check_add(&(hash_index[from][hash_value]),to,0);
					if (c==1)
						continue;
					else if (c==-1) {
						printf("Error at check_add. Terminating.\n");
						exit(-1);
					}
				}
				/* OUT structures */
				head=getListHead(out_index,from,current_out_ind_size);
				if (head>=0) {								// node exists
					last=add_neighbour(out_buffer,head,to);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==FAILURE) {
						printf("Something went wrong1\n");
						exit(-1);
					}
					else {				// (last=offset of last node of the 'linked' list)
						new=allocNewNode(out_buffer);
						if (new==-1) {			// not enough space... double buffer and try again
							if (double_buffer(&out_buffer,current_out_buf_size)==OK_SUCCESS) {
								if (TALKTOME>0)
									printf("Buffer(out) doubled!\n");
							}
							else {
								printf("FAIL: Buffer(out) realloc\n");
								exit(-1);
							}
							current_out_buf_size=2*current_out_buf_size;
							new=allocNewNode(out_buffer);
						}
						//printf("New list_node allocated\n");
						getListNode((out_buffer->base_addr)+last,&nod);
						nod.nextListNode=new;			// we link 'new' as the continuation of 'last'
						memmove((out_buffer->base_addr)+last,&nod,sizeof(list_node));	// save
						getListNode((out_buffer->base_addr)+head,&nod2);		// get 1st node
						nod2.lastListNode=new;				// 1st node points correctly to last
						memmove((out_buffer->base_addr)+head,&nod2,sizeof(list_node));
						b=add_neighbour(out_buffer,head,to);
						if (b==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (b==FAILURE) {
							printf("Something went wrong...2\n");
							exit(-1);
						}
						else {
							printf("Something went very wrong...\n");
							exit(-1);
						}
					}
				}
				else {
					if (head==-2) {				// out of bounds->double index 
						if (double_index(&out_index,from,current_out_ind_size,&multiplier)==OK_SUCCESS) {
							if (TALKTOME>0)
								printf("Index(out) doubled (x%d)!\n",multiplier);
							tmp=realloc(hash_index,multiplier*current_out_ind_size*sizeof(bucketPtr));
							if (tmp!=NULL) {
								hash_index=tmp;
								if (TALKTOME>0)
									printf("Hash index doubled (x%d)!\n",multiplier);
							}
							else {
								perror("realloc hash_index");
								exit(-1);
							}
							for (i=current_out_ind_size;i<multiplier*current_out_ind_size;i++)
								hash_index[i]=NULL;
							hash_value=hash_func(to);
							if (hash_index[from]==NULL) {
								hash_index[from]=create_hashtable();
								if (hash_index[from]==NULL)
									exit(-1);
								c=check_add(&(hash_index[from][hash_value]),to,1);
							}
							else
								c=check_add(&(hash_index[from][hash_value]),to,0);
							if (c==1)
								continue;
							else if (c==-1) {
								printf("Error at check_add. Terminating.\n");
								exit(-1);
							}
						}
						else {
							printf("FAIL: Index(out) realloc\n");
							exit(-1);
						}
						current_out_ind_size=multiplier*current_out_ind_size;
						//head=getListHead(out_index,from,current_out_ind_size);	//nomizw de xreiazetai
					}
					offset=allocNewNode(out_buffer);
					if (offset==-1) {						// double our buffer
						if (double_buffer(&out_buffer,current_out_buf_size)==OK_SUCCESS) {
							if (TALKTOME>0)
								printf("Buffer(out) doubled!\n");
						}
						else {
							printf("FAIL: Buffer(out) realloc\n");
							exit(-1);
						}
						current_out_buf_size=2*current_out_buf_size;
						offset=allocNewNode(out_buffer);
					}
					//printf("New list_node allocated\n");
					if (insertNode(out_index,from,offset)==OK_SUCCESS) {
						//printf("Node inserted to index(out)\n");
					}
					else {
						printf("Something went very wrong...\n");
						exit(-1);
					}
					last=add_neighbour(out_buffer,offset,to);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==FAILURE) {
						printf("Something went wrong...\n");
						exit(-1);
					}
					else {
						printf("Something went VERY wrong...\n");	// never will be print
						exit(-1);
					}
				}
				/* IN structures */
				head=getListHead(in_index,to,current_in_ind_size);
				if (head>=0) {								// node exists
					last=add_neighbour(in_buffer,head,from);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==FAILURE) {
						printf("Something went wrong...8\n");
						exit(-1);
					}
					else {					// (last=offset of last node of the 'linked' list)
						new=allocNewNode(in_buffer);
						if (new==-1) {			// not enough space... double buffer and try again
							if (double_buffer(&in_buffer,current_in_buf_size)==OK_SUCCESS) {
								if (TALKTOME>0)
									printf("Buffer(in) doubled!\n");
							}
							else {
								printf("FAIL: Buffer(in) realloc\n");
								exit(-1);
							}
							current_in_buf_size=2*current_in_buf_size;
							new=allocNewNode(in_buffer);
						}
						//printf("New list_node allocated\n");
						getListNode((in_buffer->base_addr)+last,&nod);
						nod.nextListNode=new;			// we link 'new' as the continuation of 'last'
						memmove((in_buffer->base_addr)+last,&nod,sizeof(list_node));	// save
						getListNode((in_buffer->base_addr)+head,&nod);		// get 1st node
						nod.lastListNode=new;			// 1st node points correctly to last
						memmove((in_buffer->base_addr)+head,&nod,sizeof(list_node));	// save
						b=add_neighbour(in_buffer,head,from);
						if (b==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (b==FAILURE) {
							printf("Something went wrong...\n");
							exit(-1);
						}
						else {
							printf("Something went very wrong...\n");
							exit(-1);
						}
					}
				}
				else {
					if (head==-2) {						// out of bounds->double index 
						if (double_index(&in_index,to,current_in_ind_size,&multiplier)==OK_SUCCESS) {
							if (TALKTOME>0)
								printf("Index(in) doubled!\n");
						}
						else {
							printf("FAIL: Index(in) realloc\n");
							exit(-1);
						}
						current_in_ind_size=multiplier*current_in_ind_size;
						//head=getListHead(in_index,to,current_in_ind_size);
					}
					offset=allocNewNode(in_buffer);
					if (offset==-1) {						// double our buffer
						if (double_buffer(&in_buffer,current_in_buf_size)==OK_SUCCESS) {
							if (TALKTOME>0)
								printf("Buffer(in) doubled!\n");
						}
						else {
							printf("FAIL: Buffer(in) realloc\n");
							exit(-1);
						}
						current_in_buf_size=2*current_in_buf_size;
						offset=allocNewNode(in_buffer);
					}
					//printf("New list_node allocated\n");
					if (insertNode(in_index,to,offset)==OK_SUCCESS) {
						//printf("Node inserted to index(in)\n");
					}
					else {
						printf("Something went very wrong...\n");
						exit(-1);
					}
					last=add_neighbour(in_buffer,offset,from);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==FAILURE) {
						printf("Something went wrong...\n");
						exit(-1);
					}
					else {
						printf("Something went VERY wrong...\n");	// never will be print
						exit(-1);
					}
				}
				if (cc_initiated==1) {
					ret=insertNewEdge(components,from,to);
					while (ret==-1 || ret==-2 || ret==-3) {
						if (ret==-1) {			// CCindex needs realloc
							if ((realloc_CCindex(components,2))==-1) {
								printf("FAIL: realloc CCindex\n");
								exit(-1);
							}
							else {
								//printf("CCindex doubled!\n");
							}
						}
						else if (ret==-2) {		// updateIndex needs realloc
							if ((realloc_updateIndex(components,2))==-1) {
								printf("FAIL: realloc updateIndex\n");
								exit(-1);
							}
							else {
								//printf("updateIndex doubled!\n");
							}
						}
						else if (ret==-3) {		// priorityIndex needs realloc
							if ((realloc_priorityIndex(components,2,0))==-1) {
								printf("FAIL: realloc priorityIndex\n");
								exit(-1);
							}
							else {
								//printf("priorityIndex doubled!\n");
							}
						}
						ret=insertNewEdge(components,from,to);
					}
					if (ret==FAILURE) {
						printf("FAIL: CC::insertNewEdge\n");
						exit(-1);
					}
				}
			}
		} /* Job executed */
	}
}



int main(int argc, char *argv[]) {

	int i;

	/* Sanity check */
	if(argc < 3) {
		printf("Error: Missing files\n");
		return -1;
	}

	/* Arxikopoihsh domwn */
	current_out_buf_size=STARTING_BUF_SIZE;				// posa struct list_node xwrane
	current_out_ind_size=STARTING_IND_SIZE;
	current_in_buf_size=STARTING_BUF_SIZE;
	current_in_ind_size=STARTING_IND_SIZE;
	GLOBAL_VISIT=0;
	out_index=createNodeIndex();
	in_index=createNodeIndex();
	if (TALKTOME>0)
		printf("In+out indexes created!\n");
	out_buffer=createBuffer();
	in_buffer=createBuffer();
	if (TALKTOME>0)
		printf("In+out buffers created!\n");
	hash_index=malloc(current_out_ind_size*sizeof(bucketPtr));
	for (i=0;i<current_out_ind_size;i++)
		hash_index[i]=NULL;
	if (TALKTOME>0)
		printf("Hash index created!\n");
	scc_initiated=grail_initiated=cc_initiated=no_more_jobs=0;
	oura_is_empty=1;
	dyn=sta=queries_that_use_updateIndex=queries=bNode=0;
	components=NULL;
	SCC=NULL;
	grailIndex=NULL;
	scheduler=NULL;

	/* Anoigma arxeiwn gia diavasma */
	fp1 = fopen(argv[1], "r");
	if(fp1 == NULL) {
		perror("fopen");
		printf("Error: Couldn't open file %s\n", argv[1]);
		return -1;
	}
	if (TALKTOME>0)
		printf("File: %s opened successfully\n", argv[1]);
	fp2 = fopen(argv[2], "r");
	if(fp2 == NULL) {
		perror("fopen");
		printf("Error: Couldn't open file %s\n", argv[2]);
		fclose(fp1);
		return -1;
	}
	if (TALKTOME>0)
		printf("File: %s opened successfully\n\n", argv[2]);
	fp = fp1;
	
	/* Arxikopoihsh JobScheduler (here we spawn threads + initialize mutexes-cond_vars)*/
	scheduler=initialize_scheduler(THREAD_POOL_SIZE);
	if (scheduler==NULL)
		return -1;
	else if (TALKTOME>0)
		printf("Job scheduler created!\n");


	/*------- Analambanoun ta threads -------*/


	/* Destroy everything */
	printf("\n");
	if (destroy_scheduler(scheduler)==-1)	// Here master thread is joined
		printf("FAIL: Destroy JobScheduler\n");
	else if (TALKTOME>0)
		printf("Job scheduler destroyed...\n");
	if (dyn == 1 && components!=NULL) {
		free(visited);
		destroyConnectedComponents(components);
		if (TALKTOME>0)
			printf("CC destroyed...\n");
	}
	else if(sta == 1) {
		if (grailIndex!=NULL) {
			if ((destroyGrailIndex(grailIndex,SCC->comp_counts))==FAILURE )
				printf("FAIL: Destroy Grail index\n");
			else if (TALKTOME>0)
				printf("Grail index destroyed...\n");
		}
		if(destroyStronglyConnectedComponents(SCC) == OK_SUCCESS && TALKTOME>0)
			printf("Master, SCC has been deleted successfuly\n");
		else
			printf("Master, we couldn't delete your SCC struct\n");
	}
	for (i=0;i<current_out_ind_size;i++) {
		if (hash_index[i]!=NULL)
			destroy_hash(hash_index[i]);
	}
	free(hash_index);
	if (TALKTOME>0)
		printf("Hash index destroyed...\n");
	if (destroyBuffer(out_buffer)==OK_SUCCESS) {
		if (TALKTOME>0)
			printf("Buffer(out) destroyed...\n");
	}
	else
		printf("FAIL: Buffer(out) destruction\n");
	if (destroyBuffer(in_buffer)==OK_SUCCESS) {
		if (TALKTOME>0)
			printf("Buffer(in) destroyed...\n");
	}
	else
		printf("FAIL: Buffer(in) destruction\n");
	if (destroyNodeIndex(out_index)==OK_SUCCESS) {
		if (TALKTOME>0)
			printf("Index(out) destroyed...\n");
	}
	else
		printf("FAIL: Index(out) destruction\n");
	if (destroyNodeIndex(in_index)==OK_SUCCESS) {
		if (TALKTOME>0)
			printf("Index(in) destroyed...\n");
	}
	else
		printf("FAIL: Index(in) destruction\n");
		
	/* Close remaining file */
	if(fclose(fp)!=0) {
		printf("FAIL: Mas kanei nera to 2o arxeio\n");
		return -1;
	}
	
	return 0;
}
