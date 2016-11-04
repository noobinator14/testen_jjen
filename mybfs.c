#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "buffer.h"
#include "index.h"
#include "Pointer_List.h"
#include "mybfs.h"

int mybfs (uint32_t from, uint32_t to, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index,
		int current_out_ind_size, int current_in_ind_size, info_deikti *next_nodes) {

	int node1, node2, i, a, distance=1, current, offset, visit_size;
	list_node nod1, nod2, next1, nod, next;
	unsigned char *visited;

	if (from==to)
		return 0;
	node1=getListHead(out_index,from,current_out_ind_size);
	if (node1==-1 || node1==-2)				// node1 doesn't exist
		return -1;
	getListNode((out_buffer->base_addr)+node1,&nod1);	// make sure node1 has at least 1 outgoing edge
	if (nod1.neighbor[0]==-1)
		return -1;
	node2=getListHead(in_index,to,current_in_ind_size);
	if (node2==-1 || node2==-2)				// node2 doesn't exist
		return -1;
	getListNode((in_buffer->base_addr)+node2,&nod2);	// make sure node2 has at least 1 ingoing edge
	if (nod2.neighbor[0]==-1)
		return -1;

/* Elegxos se ba8os 1 */
	for (i=0;i<NEIGHB;i++) {
		if (nod1.neighbor[i]==-1)
			break;
		else if (nod1.neighbor[i]==to)
			return distance;
		else if (nod1.neighbor[i]>=0){
			if ( (eisagogi_telos(next_nodes,nod1.neighbor[i])) < 0)
				return -2;
		}
	}
	if (nod1.nextListNode>=0) {
		getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
		while (1) {
			for (i=0;i<NEIGHB;i++) {
				if (next1.neighbor[i]==-1)
					break;
				else if (next1.neighbor[i]==to)
					return distance;
				else if (next1.neighbor[i]>=0) {
					if ( (eisagogi_telos (next_nodes,next1.neighbor[i])) < 0)
						return -2;
				}
			}
			a=next1.nextListNode;
			if (a>=0)
				getListNode((out_buffer->base_addr)+a,&next1);
			else
				break;
		}
	}
	if (LIST_keni(*next_nodes))
		return -1;

/* Desmeush xwrou gia visited kai arxikopoihsh */
	if (current_out_ind_size>current_in_ind_size) {
		visit_size=current_out_ind_size;
		visited=malloc(visit_size*sizeof(unsigned char));
		for (i=0;i<visit_size;i++)
			visited[i]=0;
	}
	else {
		visit_size=current_in_ind_size;
		visited=malloc(visit_size*sizeof(unsigned char));
		for (i=0;i<visit_size;i++)
			visited[i]=0;
	}
	if (from<visit_size)
		visited[from]=1;				// mark as visited
	else {
		printf("BFS: out of 'visited' bounds\n");
		free(visited);
		return -3;
	}

/* shmadi gia au3hsh va8ous */
	if ( (eisagogi_telos(next_nodes,-3) ) < 0 ) {
		free(visited);
		return -2;
	}

	distance++;
/* Elegxos se megalutero ba8os */
	while (!LIST_keni(*next_nodes)) {
		current=eksagogi_arxi(next_nodes);
		if (current==-3) {
			if (LIST_keni(*next_nodes)) {
				free(visited);
				return -1;
			}
			else {
				distance++;
				if ( (eisagogi_telos (next_nodes,-3) ) < 0) {				// shmadi gia au3hsh va8ous
					free(visited);
					return -2;
				}
			}
		}
		else {
			if (current>=visit_size) {
				printf("BFS: out of 'visited' bounds\n");
				free(visited);
				return -3;
			}
			if (visited[current]==0) {
				offset=getListHead(out_index,current,current_out_ind_size);
				if (offset>=0) {
					getListNode((out_buffer->base_addr)+offset,&nod);
					for (i=0;i<NEIGHB;i++) {
						if (nod.neighbor[i]==-1)
							break;
						else if (nod.neighbor[i]==to) {
							free(visited);
							return distance;
						}
						else if (nod.neighbor[i]>=0) {
							if ( (eisagogi_telos(next_nodes,nod.neighbor[i])) < 0) {
								free(visited);
								return -2;
							}
						}
					}
					if (nod.nextListNode!=-1) {
						getListNode((out_buffer->base_addr)+nod.nextListNode,&next);
						while (1) {
							for (i=0;i<NEIGHB;i++) {
								if (next.neighbor[i]==-1)
									break;
								else if (next.neighbor[i]==to) {
									free(visited);
									return distance;
								}
								else if (next.neighbor[i]>=0) {
									if ( (eisagogi_telos (next_nodes,next.neighbor[i])) < 0) {
										free(visited);
										return -2;
									}
								}
							}
							a=next.nextListNode;
							if (a!=-1)
								getListNode((out_buffer->base_addr)+a,&next);
							else
								break;
						}
					}
				}
				if (current<visit_size)
					visited[current]=1;
				else {
					printf("BFS: out of 'visited' bounds\n");
					free(visited);
					return -3;
				}
			}

		}


	}
	free(visited);
	return -1;						// path does not exist
}


