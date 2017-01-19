#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../buffer/buffer.h"
#include "../index/index.h"
#include "../list/Pointer_List.h"
#include "../connected_comp/connected_comp.h"
#include "mybfs.h"

int bfs_explore (CC *components, uint32_t start, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index, int current_out_ind_size,
			int current_in_ind_size, char *visited, int currentCC) {

	int node1, node2, i, a, current, offset;
	list_node nod1, next1, nod, next, nod2, next2;

	info_deikti next_nodes=LIST_dimiourgia();

	components->ccindex[start]=currentCC;			// add to weakly connected component

/* Elegxos se ba8os 1 gia e3erxomenes akmes */
	node1=getListHead(out_index,start,current_out_ind_size);
	if (node1>=0) {
		getListNode((out_buffer->base_addr)+node1,&nod1);
		for (i=0;i<NEIGHB;i++) {
			if (nod1.neighbor[i]==-1)
				break;
			else if (nod1.neighbor[i]>=0){
				components->ccindex[nod1.neighbor[i]]=currentCC;		// add to weakly connected component
				if ( (eisagogi_telos(&next_nodes,nod1.neighbor[i])) < 0) {
					LIST_katastrofi(&next_nodes);
					return -2;
				}
			}
		}
		if (nod1.nextListNode>=0) {
			getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
			while (1) {
				for (i=0;i<NEIGHB;i++) {
					if (next1.neighbor[i]==-1)
						break;
					else if (next1.neighbor[i]>=0) {
						components->ccindex[next1.neighbor[i]]=currentCC;		// add to weakly connected component
						if ( (eisagogi_telos (&next_nodes,next1.neighbor[i])) < 0) {
							LIST_katastrofi(&next_nodes);
							return -2;
						}
					}
				}
				a=next1.nextListNode;
				if (a>=0)
					getListNode((out_buffer->base_addr)+a,&next1);
				else
					break;
			}
		}
	}

/* Elegxos se ba8os 1 gia eiserxomenes akmes */
	node2=getListHead(in_index,start,current_in_ind_size);
	if (node2>=0) {
		getListNode((in_buffer->base_addr)+node2,&nod2);
		for (i=0;i<NEIGHB;i++) {
			if (nod2.neighbor[i]==-1)
				break;
			else if (nod2.neighbor[i]>=0){
				components->ccindex[nod2.neighbor[i]]=currentCC;		// add to weakly connected component
				if ( (eisagogi_telos(&next_nodes,nod2.neighbor[i])) < 0) {
					LIST_katastrofi(&next_nodes);
					return -2;
				}
			}
		}
		if (nod2.nextListNode>=0) {
			getListNode((in_buffer->base_addr)+nod2.nextListNode,&next2);
			while (1) {
				for (i=0;i<NEIGHB;i++) {
					if (next2.neighbor[i]==-1)
						break;
					else if (next2.neighbor[i]>=0) {
						components->ccindex[next2.neighbor[i]]=currentCC;		// add to weakly connected component
						if ( (eisagogi_telos (&next_nodes,next2.neighbor[i])) < 0) {
							LIST_katastrofi(&next_nodes);
							return -2;
						}
					}
				}
				a=next2.nextListNode;
				if (a>=0)
					getListNode((in_buffer->base_addr)+a,&next2);
				else
					break;
			}
		}
	}

/* If no neighbours so far, it is alone */
	if (LIST_keni(next_nodes)) {
		LIST_katastrofi(&next_nodes);
		return 0;
	}

	visited[start]=49;

/* Elegxos se megalutero ba8os */
	while (!LIST_keni(next_nodes)) {
		current=eksagogi_arxi(&next_nodes);
		if (visited[current]==48) {
			/* Elegxos e3erxomenwn akmwn */
			offset=getListHead(out_index,current,current_out_ind_size);
			if (offset>=0) {
				getListNode((out_buffer->base_addr)+offset,&nod);
				for (i=0;i<NEIGHB;i++) {
					if (nod.neighbor[i]==-1)
						break;
					else if (nod.neighbor[i]>=0) {
						components->ccindex[nod.neighbor[i]]=currentCC;		// add to weakly connected component
						if ( (eisagogi_telos(&next_nodes,nod.neighbor[i])) < 0) {
							LIST_katastrofi(&next_nodes);
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
							else if (next.neighbor[i]>=0) {
								components->ccindex[next.neighbor[i]]=currentCC;	// add to weakly connected component
								if ( (eisagogi_telos (&next_nodes,next.neighbor[i])) < 0) {
									LIST_katastrofi(&next_nodes);
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
			/* Elegxos eiserxomenwn akmwn */
			offset=getListHead(in_index,current,current_in_ind_size);
			if (offset>=0) {
				getListNode((in_buffer->base_addr)+offset,&nod);
				for (i=0;i<NEIGHB;i++) {
					if (nod.neighbor[i]==-1)
						break;
					else if (nod.neighbor[i]>=0) {
						components->ccindex[nod.neighbor[i]]=currentCC;		// add to weakly connected component
						if ( (eisagogi_telos(&next_nodes,nod.neighbor[i])) < 0) {
							LIST_katastrofi(&next_nodes);
							return -2;
						}
					}
				}
				if (nod.nextListNode!=-1) {
					getListNode((in_buffer->base_addr)+nod.nextListNode,&next);
					while (1) {
						for (i=0;i<NEIGHB;i++) {
							if (next.neighbor[i]==-1)
								break;
							else if (next.neighbor[i]>=0) {
								components->ccindex[next.neighbor[i]]=currentCC;	// add to weakly connected component
								if ( (eisagogi_telos (&next_nodes,next.neighbor[i])) < 0) {
									LIST_katastrofi(&next_nodes);
									return -2;
								}
							}
						}
						a=next.nextListNode;
						if (a!=-1)
							getListNode((in_buffer->base_addr)+a,&next);
						else
							break;
					}
				}
			}
			visited[current]=49;
		}
	}
	LIST_katastrofi(&next_nodes);
	return 0;
}





