#ifndef __INDEX_H__
#define __INDEX_H__

#define STARTING_IND_SIZE 500000

#include <stdint.h>

typedef long NodeIndex;

NodeIndex* createNodeIndex();	// malloc + init

int insertNode(NodeIndex* index, uint32_t nodeId, long offset);

int getListHead(NodeIndex* index, uint32_t nodeId, int current_ind_size);	// return index[nodeId];

int destroyNodeIndex(NodeIndex* index);	// free

int double_index(NodeIndex **index, int current_ind_size);

#endif
