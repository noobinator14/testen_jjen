#include <stdlib.h>
#include <stdint.h>
#include "buffer.h"
#include "index.h"

NodeIndex* createNodeIndex() {

	int i=0;

	NodeIndex *index=malloc(STARTING_IND_SIZE*sizeof(NodeIndex));	// allocate space for 'ΙΝΙΤ_SIZE' nodes
	for (i=0;i<STARTING_IND_SIZE;i++)
		index[i]=-1;					// initialize to '-1' , means it's empty/unused
	return index;
}

int destroyNodeIndex(NodeIndex* index) {

	free(index);
	index=NULL;
	return OK_SUCCESS;
}

int getListHead(NodeIndex* index, uint32_t nodeId, int current_ind_size) {

	if (nodeId>=current_ind_size)		// out of bounds
		return -2;
	return index[nodeId];
}

int insertNode(NodeIndex* index, uint32_t nodeId, long offset) {

	if (index[nodeId]==-1) {
		index[nodeId]=offset;
		return OK_SUCCESS;
	}
	else
		return FAILURE;
}

int double_index(NodeIndex **index, int current_ind_size) {

	int i=0;

	void *tmp=realloc(*index,2*current_ind_size*sizeof(NodeIndex));
	if (tmp!=NULL)
		(*index)=tmp;
	else
		return FAILURE;
	for (i=current_ind_size;i<2*current_ind_size;i++)	// initialize newly allocated space as unused
		(*index)[i]=-1;
	return OK_SUCCESS;
}


