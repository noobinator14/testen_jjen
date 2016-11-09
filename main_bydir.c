#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "./index/index.h"
#include "./buffer/buffer.h"
#include "./Bidirectional_BFS/bydir.h"
#include "./list/Pointer_List.h"

int main(int argc, char *argv[]) {

	int u=0;
	int current_out_buf_size, current_out_ind_size, current_in_buf_size, current_in_ind_size;
	long offset, last, new, head, count=0, s_path_fo, b, neighb_exists=0;
	uint32_t from, to;
	char s[2];
	list_node nod;
	FILE *fp, *fp1, *fp2;

	if(argc < 3) {
		printf("Error: Missing files\n");
		return -1;
	}

	current_out_buf_size=STARTING_BUF_SIZE;				// posa struct list_node xwrane
	current_out_ind_size=STARTING_IND_SIZE;
	current_in_buf_size=STARTING_BUF_SIZE;
	current_in_ind_size=STARTING_IND_SIZE;

	NodeIndex* out_index=createNodeIndex();
	NodeIndex* in_index=createNodeIndex();
	//printf("In+out indexes created!\n");
	Buffer* out_buffer=createBuffer();
	Buffer* in_buffer=createBuffer();
	//printf("In+out buffers created!\n");


	fp1 = fopen(argv[1], "r");
	if(fp1 == NULL) {
		perror("fopen");
		printf("Error: Couldn't open file %s\n", argv[1]);
	}
	//printf("File: %s opened successfully\n", argv[1]);

	fp2 = fopen(argv[2], "r");
	if(fp2 == NULL) {
		perror("fopen");
		printf("Error: Couldn't open file %s\n", argv[2]);
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
			//if(count % 100000 == 0)
				//printf("Count: %ld\n", count);
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
				s_path_fo=mybfs(from, to, out_buffer, in_buffer, out_index, in_index, current_out_ind_size, current_in_ind_size, &next_nodes1, &next_nodes2);
				printf("%ld\n",s_path_fo);
				LIST_katastrofi(&next_nodes1);
				LIST_katastrofi(&next_nodes2);
			}
			//if(count % 1000 == 0)
			//	printf("Count: %ld\n", count);
		}
		neighb_exists=0;
		if((fp == fp1) || ((fp == fp2) && (strcmp(s, "A")==0))) {
			if (from>=0 && to>=0 && from!=to) {
				/* OUT structures */
				head=getListHead(out_index,from,current_out_ind_size);
				if (head>=0) {								// node exists
					last=add_neighbour(out_buffer,head,to);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==NEIGHB_EXISTS) {
						//printf("Neighbour already exists\n");
						neighb_exists=1;
					}
					else {								// (last=offset of last node of the 'linked' list)
						new=allocNewNode(out_buffer);
						if (new==-1) {						// not enough space... double buffer and try again
							if (double_buffer(&out_buffer,current_out_buf_size)==OK_SUCCESS) {
								//printf("Buffer(out) doubled!\n");
							}
							else
								printf("FAIL: Buffer(out) realloc\n");
							current_out_buf_size=2*current_out_buf_size;
							new=allocNewNode(out_buffer);
							//printf("New list_node allocated\n");
						}
						//else
							//printf("New list_node allocated\n");
						getListNode((out_buffer->base_addr)+last,&nod);
						nod.nextListNode=new;					// we link 'new' as the continuation of 'last'
						memmove((out_buffer->base_addr)+last,&nod,sizeof(list_node));
						b=add_neighbour(out_buffer,new,to);
						if (b==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (b==NEIGHB_EXISTS)
							printf("Something went wrong...\n");
						else
							printf("Something went very wrong...\n");
					}
				}
				else {
					if (head==-2) {						// out of bounds->double index until nodeId fits into index
						while (head==-2) {
							if (double_index(&out_index,current_out_ind_size)==OK_SUCCESS) {
								//printf("Index(out) doubled!\n");
							}
							else
								printf("FAIL: Index(out) realloc\n");
							current_out_ind_size=2*current_out_ind_size;
							head=getListHead(out_index,from,current_out_ind_size);
						}
					}
					offset=allocNewNode(out_buffer);
					if (offset==-1) {						// double our buffer
						if (double_buffer(&out_buffer,current_out_buf_size)==OK_SUCCESS) {
							//printf("Buffer(out) doubled!\n");
						}
						else
							printf("FAIL: Buffer(out) realloc\n");
						current_out_buf_size=2*current_out_buf_size;
						offset=allocNewNode(out_buffer);
						//printf("New list_node allocated\n");
					}
					//else
						//printf("New list_node allocated\n");
					if (insertNode(out_index,from,offset)==OK_SUCCESS) {
						//printf("Node inserted to index(out)\n");
					}
					else
						printf("Something went very wrong...\n");
					last=add_neighbour(out_buffer,offset,to);
					if (last==OK_SUCCESS) {
						//printf("Neighbour added\n");
					}
					else if (last==NEIGHB_EXISTS)
						printf("Something went wrong...\n");
					else
						printf("I WILL NEVER BE PRINT!\n");
				}
				if (neighb_exists==0) {
					/* IN structures */
					head=getListHead(in_index,to,current_in_ind_size);
					if (head>=0) {								// node exists
						last=add_neighbour(in_buffer,head,from);
						if (last==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (last==NEIGHB_EXISTS) {
							//printf("Neighbour already exists\n");
						}
						else {								// (last=offset of last node of the 'linked' list)
							new=allocNewNode(in_buffer);
							if (new==-1) {						// not enough space... double buffer and try again
								if (double_buffer(&in_buffer,current_in_buf_size)==OK_SUCCESS) {
									//printf("Buffer(in) doubled!\n");
								}
								else
									printf("FAIL: Buffer(in) realloc\n");
								current_in_buf_size=2*current_in_buf_size;
								new=allocNewNode(in_buffer);
								//printf("New list_node allocated\n");
							}
							//else
								//printf("New list_node allocated\n");
							getListNode((in_buffer->base_addr)+last,&nod);
							nod.nextListNode=new;					// we link 'new' as the continuation of 'last'
							memmove((in_buffer->base_addr)+last,&nod,sizeof(list_node));
							b=add_neighbour(in_buffer,new,from);
							if (b==OK_SUCCESS) {
								//printf("Neighbour added\n");
							}
							else if (b==NEIGHB_EXISTS)
								printf("Something went wrong...\n");
							else
								printf("Something went very wrong...\n");
						}
					}
					else {
						if (head==-2) {						// out of bounds->double index until nodeId fits into index
							while (head==-2) {
								if (double_index(&in_index,current_in_ind_size)==OK_SUCCESS) {
									//printf("Index(in) doubled!\n");
								}
								else
									printf("FAIL: Index(in) realloc\n");
								current_in_ind_size=2*current_in_ind_size;
								head=getListHead(in_index,to,current_in_ind_size);
							}
						}
						offset=allocNewNode(in_buffer);
						if (offset==-1) {						// double our buffer
							if (double_buffer(&in_buffer,current_in_buf_size)==OK_SUCCESS) {
								//printf("Buffer(in) doubled!\n");
							}
							else
								printf("FAIL: Buffer(in) realloc\n");
							current_in_buf_size=2*current_in_buf_size;
							offset=allocNewNode(in_buffer);
							//printf("New list_node allocated\n");
						}
						//else
							//printf("New list_node allocated\n");
						if (insertNode(in_index,to,offset)==OK_SUCCESS) {
							//printf("Node inserted to index(in)\n");
						}
						else
							printf("Something went very wrong...\n");
						last=add_neighbour(in_buffer,offset,from);
						if (last==OK_SUCCESS) {
							//printf("Neighbour added\n");
						}
						else if (last==NEIGHB_EXISTS)
							printf("Something went wrong...\n");
						else
							printf("I WILL NEVER BE PRINT!\n");
					}
				}
			}
		}
	}

	if(fclose(fp) == -1) {
		printf("Error: Mas kanei nera to 2o arxeio\n");
		return -10;
	}

	printf("\n");
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
