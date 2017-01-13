#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Stack.h"
#include "../strongly_connected_comp/scc.h"

uint32_t min(uint32_t a, uint32_t b) {
	if (a<b)
		return a;
	else
		return b;
}

scc * estimateSCC(Buffer *out_buffer,Buffer *in_buffer,NodeIndex *out_index,NodeIndex *in_index,int current_out_ind_size,int current_in_ind_size,int bNode) {
	int i,k,a,index,out_off,in_off;
	uint32_t popped;
	list_node nod1,next1;

	scc *SCC=malloc(sizeof(scc));
	SCC->comp=NULL;
	SCC->inS=NULL;
	SCC->comp_counts=0;				// how many SCC's we have now
	SCC->id_belongs_to_comp=malloc(bNode*sizeof(uint32_t));
	for (i=0;i<bNode;i++)
		SCC->id_belongs_to_comp[i]=-1;		// belongs in no SCC
	//char *visited=malloc(bNode*sizeof(char));
	//for (i=0;i<bNode;i++)
	//	visited[i]=0;
	infoSCC *info=malloc(bNode*sizeof(infoSCC));
	for (i=0;i<bNode;i++) {
		info[i].lowLink=-1;
		info[i].rank=-1;
		info[i].onS=0;
	}
	uint32_t *parent=malloc(bNode*sizeof(uint32_t));	// helps us return from recursion
	for (i=0;i<bNode;i++)
		parent[i]=-1;

	int onRecursion=0;

	index=0;
	stoiva *s=Stoiva_dimiourgia(bNode);
	for (i=0;i<bNode;i++) {
		//if (visited[i]==0) {
		//	visited[i]=1;
		out_off=getListHead(out_index,i,current_out_ind_size);
		in_off=getListHead(in_index,i,current_in_ind_size);
		if ((out_off>=0 || in_off>=0) && info[i].rank==-1) {
			info[i].rank=index;
			info[i].lowLink=index;
			index++;
			if ((Stoiva_push(s,i))==-1) {
				printf("FAIL: stack full, can't push\n");
				return NULL;
			}
			info[i].onS=1;
			if (out_off>=0) {
				getListNode((out_buffer->base_addr)+out_off,&nod1);
				for (k=0;k<NEIGHB;k++) {
					if (nod1.neighbor[k]==-1)
						break;
					else {
						if (info[nod1.neighbor[k]].rank==-1) {
							// strongconnect(w)...
							parent[nod1.neighbor[k]]=i;
							

							info[i].lowLink=min(info[i].lowLink,info[nod1.neighbor[k]].lowLink);
						}
						else if (info[nod1.neighbor[k]].onS==1) {
							info[i].lowLink=min(info[i].lowLink,info[nod1.neighbor[k]].rank);
						}
					}
				}
				if (nod1.nextListNode>=0) {
					getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
					while (1) {
						for (k=0;k<NEIGHB;k++) {
							if (next1.neighbor[k]==-1)
								break;
							else {
								if (info[next1.neighbor[k]].rank==-1) {
									// strongconnect(w)...


									info[i].lowLink=min(info[i].lowLink,info[next1.neighbor[k]].lowLink);
								}
								else if (info[next1.neighbor[k]].onS==1) {
									info[i].lowLink=min(info[i].lowLink,info[next1.neighbor[k]].rank);
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
			if (info[i].lowLink==info[i].rank) {			// generate SCC
				do {
					popped=Stoiva_pop(s);			// elegxos an vgalei -1?
					info[popped].onS=0;
					SCC->id_belongs_to_comp[popped]=SCC->comp_counts;
				} while (popped!=i);
				SCC->comp_counts++;
			}
		}
		//}
	}



	printf("Found %d SCC\n",SCC->comp_counts);
	SCC->inS=info;
	Stoiva_katastrofi(s);
	return SCC;
}
