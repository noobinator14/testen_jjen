#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "./index/index.h"
#include "./buffer/buffer.h"
#include "./Bidirectional_BFS_SCC/bydir.h"
#include "./list/Pointer_List.h"
#include "./hashtable/hash.h"
#include "./strongly_connected_comp/scc.h"

int main(int argc, char *argv[]) {

	int current_out_buf_size, current_out_ind_size, current_in_buf_size, current_in_ind_size, i, u=0, multiplier=1, hash_value;
	long offset, last, new, head, count=0, s_path_fo, b, c;
	uint32_t from, to, bNode = 0;
	char s[2];
	void *tmp;

	list_node nod, nod2;

	scc *SCC;

	FILE *fp, *fp1, *fp2;

	if(argc < 3) {
		printf("Error: Missing files\n");
		return -1;
	}

	current_out_buf_size=STARTING_BUF_SIZE;				// posa struct list_node xwrane (polla!)
	current_out_ind_size=STARTING_IND_SIZE;
	current_in_buf_size=STARTING_BUF_SIZE;
	current_in_ind_size=STARTING_IND_SIZE;

	NodeIndex* out_index=createNodeIndex();
	NodeIndex* in_index=createNodeIndex();
	//printf("In+out indexes created!\n");
	Buffer* out_buffer=createBuffer();
	Buffer* in_buffer=createBuffer();
	//printf("In+out buffers created!\n");
	bucketPtr *hash_index=malloc(current_out_ind_size*sizeof(bucketPtr));
	for (i=0;i<current_out_ind_size;i++)	//Giati mono sto out...
		hash_index[i]=NULL;
	printf("Hash index created!\n");

	fp1 = fopen(argv[1], "r");
	if(fp1 == NULL) {
		perror("fopen");
		printf("Error: Couldn't open file %s\n", argv[1]);
		return -1;
	}
	//printf("File: %s opened successfully\n", argv[1]);

	fp2 = fopen(argv[2], "r");
	if(fp2 == NULL) {
		perror("fopen");
		printf("Error: Couldn't open file %s\n", argv[2]);
		fclose(fp1);
		return -1;
	}
	//printf("File: %s opened successfully\n\n", argv[2]);
	fp = fp1;
	while(1) {
		if(fscanf(fp, "%1s", s) == EOF) {	//Reads only 1 char of the first word
			if(u==0) {
				fp = fp2;
				count=0;
				if(fclose(fp1) != 0) {
					perror("fclose");
					return -1;
				}
				u++;
				continue;
			}
			else {
				break;
			}
		}
		count++;
		if(fp == fp1) {
			if(strcmp(s, "S") == 0) {
				fp = fp2;
				count=0;
				if(fclose(fp1) != 0) {
					perror("fclose");
					return -1;
				}
				u++;
				//Εδώ καλούμε τον αλγόριθμο για τα S.C.C.
				printf("Master, initiating SCC functions\n");
				SCC = estimatedStronglyConnectComp(out_buffer, in_buffer, out_index, in_index, current_out_ind_size, current_in_ind_size, bNode);
				if(SCC == NULL) {
					return -9;
				}
				else {
					printf("Master, SCC is ready\n");
					getchar();
				}
				continue;
			}
			else {
				if(fseek(fp, -1, SEEK_CUR) != 0) {	//Goes back at the start of the line
					perror("fseek");
					return -1;
				}
				if(fscanf(fp, "%u %u", &from, &to) == EOF) {	//Reads both nodes
					break;
				}
			}
			if(count % 100000 == 0)
				printf("Count: %ld\n", count);
		}
		else if(fp == fp2) {
			if(strcmp(s, "F") == 0) {
				continue;
			}
			if(fscanf(fp, "%u %u", &from, &to) == EOF) {	//Reads both nodes
				break;
			}
			if(strcmp(s, "Q") == 0) {
				info_deikti next_nodes1=LIST_dimiourgia();
				info_deikti next_nodes2=LIST_dimiourgia();
				if (next_nodes1==NULL || next_nodes2==NULL)
					return -1;
				s_path_fo = estimateShortestPathStronglyConnectedComponents(SCC, out_buffer, in_buffer, out_index, in_index, current_out_ind_size, current_in_ind_size, from, to, 												&next_nodes1, &next_nodes2);
				//s_path_fo=mybfs(from, to, out_buffer, in_buffer, out_index, in_index, current_out_ind_size, current_in_ind_size, &next_nodes1, &next_nodes2);
				if (s_path_fo==-2) {
					printf("Error inside bidirectional.\nCount=%ld. Terminating.\n",count);
					return -1;
				}
				else
					printf("%ld\n",s_path_fo);
				LIST_katastrofi(&next_nodes1);
				LIST_katastrofi(&next_nodes2);
			}
			//if(count % 1000 == 0)
			//	printf("Count: %ld\n", count);
		}
		if((fp == fp1) || ((fp == fp2) && (strcmp(s, "A")==0))) {
			if (from>=0 && to>=0 && from!=to) {
				if(from > bNode) {
					bNode = from;
				}
				else if(to > bNode) {
					bNode = to;
				}
				if (from<current_out_ind_size) {
					hash_value=hash_func(to);
					if (hash_index[from]==NULL) {
						hash_index[from]=create_hashtable();
						if (hash_index[from]==NULL)
							return -1;
						c=check_add(&(hash_index[from][hash_value]),to,1);
					}
					else
						c=check_add(&(hash_index[from][hash_value]),to,0);
					if (c==1)
						continue;
					else if (c==-1) {
						printf("Error at check_add. Terminating.\n");
						return -1;
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
						return -1;
					}
					else {								// (last=offset of last node of the 'linked' list)
						new=allocNewNode(out_buffer);
						if (new==-1) {						// not enough space... double buffer and try again
							if (double_buffer(&out_buffer,current_out_buf_size)==OK_SUCCESS) {
								printf("Buffer(out) doubled!\n");
							}
							else {
								printf("FAIL: Buffer(out) realloc\n");
								return -1;
							}
							current_out_buf_size=2*current_out_buf_size;
							new=allocNewNode(out_buffer);
						}
						//printf("New list_node allocated\n");
						getListNode((out_buffer->base_addr)+last,&nod);
						nod.nextListNode=new;					// we link 'new' as the continuation of 'last'
						memmove((out_buffer->base_addr)+last,&nod,sizeof(list_node));	// save
						getListNode((out_buffer->base_addr)+head,&nod2);		// get 1st node
						nod2.lastListNode=new;					// 1st node points correctly to last
						memmove((out_buffer->base_addr)+head,&nod2,sizeof(list_node));
						b=add_neighbour(out_buffer,head,to);
						if (b==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (b==FAILURE) {
							printf("Something went wrong...2\n");
							return -1;
						}
						else {
							printf("Something went very wrong...\n");
							return -1;
						}
					}
				}
				else {
					if (head==-2) {						// out of bounds->double index 
						if (double_index(&out_index,from,current_out_ind_size,&multiplier)==OK_SUCCESS) {
							printf("Index(out) doubled (x%d)!\n",multiplier);
							tmp=realloc(hash_index,multiplier*current_out_ind_size*sizeof(bucketPtr));
							if (tmp!=NULL) {
								hash_index=tmp;
								printf("Hash index doubled (x%d)!\n",multiplier);
							}
							else {
								perror("realloc hash_index");
								return -1;
							}
							for (i=current_out_ind_size;i<multiplier*current_out_ind_size;i++)
								hash_index[i]=NULL;
							hash_value=hash_func(to);
							if (hash_index[from]==NULL) {
								hash_index[from]=create_hashtable();
								if (hash_index[from]==NULL)
									return -1;
								c=check_add(&(hash_index[from][hash_value]),to,1);
							}
							else
								c=check_add(&(hash_index[from][hash_value]),to,0);
							if (c==1)
								continue;
							else if (c==-1) {
								printf("Error at check_add. Terminating.\n");
								return -1;
							}
						}
						else {
							printf("FAIL: Index(out) realloc\n");
							return -1;
						}
						current_out_ind_size=multiplier*current_out_ind_size;
						//head=getListHead(out_index,from,current_out_ind_size);	//nomizw de xreiazetai
					}
					offset=allocNewNode(out_buffer);
					if (offset==-1) {						// double our buffer
						if (double_buffer(&out_buffer,current_out_buf_size)==OK_SUCCESS) {
							printf("Buffer(out) doubled!\n");
						}
						else {
							printf("FAIL: Buffer(out) realloc\n");
							return -1;
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
						return -1;
					}
					last=add_neighbour(out_buffer,offset,to);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==FAILURE) {
						printf("Something went wrong...\n");
						return -1;
					}
					else {
						printf("Something went VERY wrong...\n");	// never will be print
						return -1;
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
						return -1;
					}
					else {								// (last=offset of last node of the 'linked' list)
						new=allocNewNode(in_buffer);
						if (new==-1) {						// not enough space... double buffer and try again
							if (double_buffer(&in_buffer,current_in_buf_size)==OK_SUCCESS) {
								printf("Buffer(in) doubled!\n");
							}
							else {
								printf("FAIL: Buffer(in) realloc\n");
								return -1;
							}
							current_in_buf_size=2*current_in_buf_size;
							new=allocNewNode(in_buffer);
						}
						//printf("New list_node allocated\n");
						getListNode((in_buffer->base_addr)+last,&nod);
						nod.nextListNode=new;					// we link 'new' as the continuation of 'last'
						memmove((in_buffer->base_addr)+last,&nod,sizeof(list_node));	// save
						getListNode((in_buffer->base_addr)+head,&nod);		// get 1st node
						nod.lastListNode=new;					// 1st node points correctly to last
						memmove((in_buffer->base_addr)+head,&nod,sizeof(list_node));	// save
						b=add_neighbour(in_buffer,head,from);
						if (b==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (b==FAILURE) {
							printf("Something went wrong...\n");
							return -1;
						}
						else {
							printf("Something went very wrong...\n");
							return -1;
						}
					}
				}
				else {
					if (head==-2) {						// out of bounds->double index 
						if (double_index(&in_index,to,current_in_ind_size,&multiplier)==OK_SUCCESS) {
							printf("Index(in) doubled!\n");
						}
						else {
							printf("FAIL: Index(in) realloc\n");
							return -1;
						}
						current_in_ind_size=multiplier*current_in_ind_size;
						//head=getListHead(in_index,to,current_in_ind_size);
					}
					offset=allocNewNode(in_buffer);
					if (offset==-1) {						// double our buffer
						if (double_buffer(&in_buffer,current_in_buf_size)==OK_SUCCESS) {
							printf("Buffer(in) doubled!\n");
						}
						else {
							printf("FAIL: Buffer(in) realloc\n");
							return -1;
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
						return -1;
					}
					last=add_neighbour(in_buffer,offset,from);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==FAILURE) {
						printf("Something went wrong...\n");
						return -1;
					}
					else {
						printf("Something went VERY wrong...\n");	// never will be print
						return -1;
					}
				}
			}
		}
	}

	if(fclose(fp) == -1) {
		printf("Error: Mas kanei nera to 2o arxeio\n");
		return -1;
	}

	printf("\n");
	for (i=0;i<current_out_ind_size;i++) {
		if (hash_index[i]!=NULL)
			destroy_hash(hash_index[i]);
	}
	free(hash_index);
	printf("Hash index destroyed...\n");
	if(destroyStronglyConnectedComponents(SCC) == OK_SUCCESS)
		printf("Master, SCC has been deleted successfuly\n");
	else
		printf("Master, we couldn't delete your SCC struct\n");
	printf("SJiet\n");
	if (destroyBuffer(out_buffer)==OK_SUCCESS)
		printf("Buffer(out) destroyed...\n");
	else
		printf("FAIL: Buffer(out) destruction\n");
	if (destroyBuffer(in_buffer)==OK_SUCCESS)
		printf("Buffer(in) destroyed...\n");
	else
		printf("FAIL: Buffer(in) destruction\n");
	if (destroyNodeIndex(out_index)==OK_SUCCESS)
		printf("Index(out) destroyed...\n");
	else
		printf("FAIL: Index(out) destruction\n");
	if (destroyNodeIndex(in_index)==OK_SUCCESS)
		printf("Index(in) destroyed...\n");
	else
		printf("FAIL: Index(in) destruction\n");
	return 0;
}
