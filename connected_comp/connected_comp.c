#include <stdio.h>
#include <stdlib.h>
#include "../buffer/buffer.h"
#include "../index/index.h"
#include "../BFS/mybfs.h"
#include "connected_comp.h"

int CC_init(CC *components, int ccindex_size) {

	int i;

	(components->ccindex)=malloc(ccindex_size * sizeof(uint32_t));
	if ( (components->ccindex)==NULL ) {
		printf("FAIL: malloc CC index\n");
		return -1;
	}
	(components->updateIndex)=malloc(ccindex_size * sizeof(info_deikti));
	if ( (components->updateIndex)==NULL ) {
		printf("FAIL: malloc Update index\n");
		free(components->ccindex);
		return -1;
	}
	components->priorityIndex=NULL;
	components->priorityIndexSize=0;
	components->metric=0;
	components->ccindexSize=ccindex_size;
	components->updateIndexSize=ccindex_size;
	components->next_CC=0;
	for (i=0;i<ccindex_size;i++) {
		(components->ccindex)[i]=-1;
		(components->updateIndex)[i]=NULL;
	}
	return 0;
}

int destroyConnectedComponents(CC *components) {
	int i;

	for (i=0;i<(components->updateIndexSize);i++) {
		if ((components->updateIndex)[i]!=NULL)
			LIST_katastrofi(&((components->updateIndex)[i]));
	}
	free(components->updateIndex);
	free(components->priorityIndex);
	free(components->ccindex);
	free(components);
	return OK_SUCCESS;
}

CC * estimateConnectedComponents(NodeIndex *out_index, NodeIndex *in_index, Buffer *out_buffer, Buffer *in_buffer, int curr_out_ind_size, int curr_in_ind_size) {

	int i, currentCC=0, visit_size, ret, head;
	uint32_t currentNodeId;
	char *visited;

	CC *components=malloc(sizeof(CC));
	if (components==NULL) {
		printf("FAIL: malloc weakly CC\n");
		return NULL;
	}
	if (curr_out_ind_size >= curr_in_ind_size)
		visit_size=curr_out_ind_size;
	else
		visit_size=curr_in_ind_size;
	if ( (CC_init(components,visit_size)) < 0 )
		return NULL;
	visited=malloc(visit_size * sizeof(char));
	for (i=0; i<visit_size; i++)
		visited[i]=48;			// ASCII code for '0'

	/* For every node that has outgoing edges */
	for (currentNodeId=0; currentNodeId<curr_out_ind_size; currentNodeId++) {
		head=getListHead(out_index,currentNodeId,curr_out_ind_size);
		if (head>=0) {
			if (visited[currentNodeId]==48) {
				ret=bfs_explore(components,currentNodeId,out_buffer,in_buffer,out_index,in_index, 
						curr_out_ind_size,curr_in_ind_size,visited,currentCC);
				if (ret==-1 || ret==-2) {
					printf("Something went wrong...1\n");
					free(visited);
					return NULL;
				}
				currentCC++;
			}
		}
	}

	/* For every node that has ingoing edges */
	for (currentNodeId=0; currentNodeId<curr_in_ind_size; currentNodeId++) {
		head=getListHead(in_index,currentNodeId,curr_in_ind_size);
		if (head>=0) {
			if (visited[currentNodeId]==48) {
				ret=bfs_explore(components,currentNodeId,out_buffer,in_buffer,out_index,in_index, 
						curr_out_ind_size,curr_in_ind_size,visited,currentCC);
				if (ret==-1 || ret==-2) {
					printf("Something went wrong...2\n");
					free(visited);
					return NULL;
				}
				currentCC++;
			}
		}
	}
	components->next_CC=currentCC;
	free(visited);
	if (components->priorityIndex==NULL) {
		(components->priorityIndex)=malloc((components->next_CC)*sizeof(int));
		if (components->priorityIndex==NULL)
			return NULL;
		components->priorityIndexSize=(components->next_CC);
		for (i=0;i<(components->next_CC);i++)
			(components->priorityIndex)[i]=-1;
	}
	return components;
}

int findNodeCCid(CC *components, uint32_t nodeId) {
	if (nodeId < (components->ccindexSize) )
		return components->ccindex[nodeId];
	else
		return -1;
}

int insertNewEdge(CC *components, uint32_t nodeIdS, uint32_t nodeIdE) {
/* SYMBASH: sygxwneush sto CC me th mikroterh timh */
//printf("Entered insert\n");
	int big, small, prior;

	if (nodeIdS>=(components->ccindexSize) || nodeIdE>=(components->ccindexSize))			// CCindex needs realloc
		return -1;
	if (nodeIdS>=0 && nodeIdE>=0) {
		int ccS=components->ccindex[nodeIdS];
		int ccE=components->ccindex[nodeIdE];
		if (ccS>=0 && ccE>=0) {			// both nodes belong to a CC
			if (ccS<ccE) {
				small=ccS;
				big=ccE;
			}
			else if (ccE<ccS) {
				small=ccE;
				big=ccS;
			}
			else
				return OK_SUCCESS;
			if (big>=(components->priorityIndexSize))			// priorityIndex needs realloc
				return -3;
			if ((components->priorityIndex)[small]!=-1)
				prior=(components->priorityIndex)[small];		// prior: se poia lista 8a mpei
			else
				prior=small;
			if (prior==(components->priorityIndex)[big])			// already exists
				return OK_SUCCESS;
			if (prior >= (components->updateIndexSize) )			// updateIndex needs realloc
				return -2;
			if ((components->updateIndex)[prior] == NULL) {
				(components->updateIndex)[prior]=LIST_dimiourgia();
				if ((components->updateIndex)[prior]==NULL)
					return FAILURE;
			}
			if ((components->updateIndex)[big]!=NULL) {
				typos_deikti start=get_arxi((components->updateIndex)[big]);
				typos_deikti next;
				if (start!=NULL) {
					int data=periexomeno(start);
					(components->priorityIndex)[data]=prior;
					next=epomenos(start);
					while (next!=NULL) {
						data=periexomeno(next);
						(components->priorityIndex)[data]=prior;
						next=epomenos(next);
					}
					if ((eisagogi_telos(&((components->updateIndex)[prior]),big))==-1) {
						printf("Error at CC_InsertNewEdge->eisagogi_telos\n");
						return FAILURE;
					}
					merge_telos(&((components->updateIndex)[prior]),(components->updateIndex)[big]);
					LIST_katastrofi_info_node(&((components->updateIndex)[big]));
					(components->priorityIndex)[big]=prior;
				}
			}
			else {
				//printf("sa\n"); //printf("prior=%d, updateIndexSize=%d, big=%d\n",prior,components->updateIndexSize,big);
				if ((anazhthsh((components->updateIndex)[prior],big))==0) {			// sarwsh mhpws uparxei hdh enwsh
					//printf("sa2\n");
					if ((eisagogi_telos(&((components->updateIndex)[prior]),big))==-1) {			// pros8hkh enwshs
						printf("Error at CC_InsertNewEdge->eisagogi_telos\n");
						return FAILURE;
					}
					(components->priorityIndex)[big]=prior;
				}
				else {
					//printf("sa3\n");
					return OK_SUCCESS;
				}
			}
		}
		else if (ccS==-1 && ccE==-1) {		// no οne belongs to a CC
			components->ccindex[nodeIdS]=components->next_CC;
			components->ccindex[nodeIdE]=components->next_CC;
			(components->next_CC)++;
		}
		else {					// only one of them belongs to a CC
			if (ccS==-1)
				components->ccindex[nodeIdS]=(components->ccindex[nodeIdE]);
			else
				components->ccindex[nodeIdE]=(components->ccindex[nodeIdS]);
		}
	}
	else
		return FAILURE;
	return OK_SUCCESS;
}

int realloc_CCindex(CC *components, int multiplier) {
	void *tmp;
	int i,old_size;

	tmp=realloc(components->ccindex,multiplier*(components->ccindexSize)*sizeof(uint32_t));
	if (tmp!=NULL) {
		old_size=components->ccindexSize;
		(components->ccindex)=tmp;
		(components->ccindexSize)=multiplier*old_size;
	}
	else
		return -1;
	for (i=old_size;i<(components->ccindexSize);i++)
		(components->ccindex)[i]=-1;
	return 0;
}

int realloc_updateIndex(CC *components, int multiplier) {
	void *tmp;
	int i, old_size;

	tmp=realloc(components->updateIndex,multiplier*(components->updateIndexSize)*sizeof(info_deikti));
	if (tmp!=NULL) {
		old_size=components->updateIndexSize;
		(components->updateIndex)=tmp;
		(components->updateIndexSize)=multiplier*old_size;
	}
	else
		return -1;
	for (i=old_size;i<(components->updateIndexSize);i++)
		(components->updateIndex)[i]=NULL;
	return 0;
}

int realloc_priorityIndex(CC *components, int multiplier, int new_size) {
	void *tmp;
	int i, old_size;

	if (new_size==0 && multiplier>1)
		tmp=realloc(components->priorityIndex,multiplier*(components->priorityIndexSize)*sizeof(int));
	else if (multiplier==0 && new_size>0)
		tmp=realloc(components->priorityIndex,new_size*sizeof(int));
	else {
		printf("realloc_priorityIndex : invalid arguments\n");
		return -1;
	}
	if (tmp!=NULL) {
		old_size=components->priorityIndexSize;
		(components->priorityIndex)=tmp;
		(components->priorityIndexSize)=multiplier*old_size + new_size;
	}
	else
		return -1;
	for (i=old_size;i<(components->priorityIndexSize);i++)
		(components->priorityIndex)[i]=-1;
	return 0;
}

int CC_search_for_connection(CC *components, uint32_t node1, uint32_t node2, int *queries_updateIndex) {
/* Return 0: no connection  , Return 1: maybe connection */
//printf("Entered CC_search_connection\n");
	if (node1>=(components->ccindexSize) || node2>=(components->ccindexSize))
		return 0;
	int cc1=(components->ccindex)[node1];
	int cc2=(components->ccindex)[node2];
	if (cc1==-1 || cc2==-1)
		return 0;
	if (cc1==cc2)
		return 1;
	else {
		int small,big,list;
		if (cc1<cc2) {
			small=cc1;
			big=cc2;
		}
		else {
			small=cc2;
			big=cc1;
		}
		if ((components->priorityIndex)[small]==-1)
			list=small;
		else
			list=(components->priorityIndex)[small];
		if ((components->updateIndex)[list]==NULL)
			return 0;
		(*queries_updateIndex)++;
		typos_deikti start=get_arxi((components->updateIndex)[list]);
		typos_deikti next;
		if (start!=NULL) {
			int data=periexomeno(start);
			if (data==big)
				return 1;
			next=epomenos(start);
			while (next!=NULL) {
				data=periexomeno(next);
				if (data==big)
					return 1;
				next=epomenos(next);
			}
		}
	}
	return 0;
}

int rebuildIndexes(CC *components) {
	int i, old_CC_of_node, new_possible_CC_of_node;
	//int currentCC=0;

	if (components->priorityIndexSize < components->next_CC)
		if ((realloc_priorityIndex(components,0,components->next_CC))==-1)
			return FAILURE;

	/* Cover CC gaps */
	/*for (i=0;i<(components->updateIndexSize);i++) {
		if ((components->updateIndex)[i]!=NULL) {
			(components->priorityIndex)[i]=currentCC;
			typos_deikti start=get_arxi((components->updateIndex)[i]);
			typos_deikti next;
			if (start!=NULL) {
				int data=periexomeno(start);
				(components->priorityIndex)[data]=currentCC;
				next=epomenos(start);
				while (next!=NULL) {
					data=periexomeno(next);
					(components->priorityIndex)[data]=currentCC;
					next=epomenos(next);
				}
				currentCC++;
			}
		}
	} */

	/* Make the changes */
	for (i=0;i<(components->ccindexSize);i++) {
		if ( (components->ccindex)[i] != -1 ) {
			old_CC_of_node=(components->ccindex)[i];
			new_possible_CC_of_node=(components->priorityIndex)[old_CC_of_node];
			if (new_possible_CC_of_node!=-1)
				(components->ccindex)[i]=new_possible_CC_of_node;
			/*else {
				(components->ccindex)[i]=currentCC;
				currentCC++;
			} */
		}
	}

	/* Cover CC gaps (safe+slow)??? */
	/*for (i=0;i<(components->priorityIndexSize);i++)
		(components->priorityIndex)[i]=-1;
	int searching=0, missing=-1, change;
	for (i=0;i<(components->ccindexSize);i++) {
		if (missing!=-1) {
			change=(components->ccindex)[i];
			(components->priorityIndex)[change]=missing;
			
		}
		if ((components->ccindex)[i]==searching) {
			searching++;
			i=0;
		}
		if (i==(components->ccindexSize)-1) {
			missing=searching;
			i=0;
		}
	}*/


	for (i=0;i<(components->updateIndexSize);i++)
		if ((components->updateIndex)[i]!=NULL)
			LIST_katastrofi(&((components->updateIndex)[i]));	// ekka8arish updateIndex
	for (i=0;i<(components->priorityIndexSize);i++)
		(components->priorityIndex)[i]=-1;				// ekka8arish priorityIndex
	//(components->next_CC)=currentCC;
	return OK_SUCCESS;
}






