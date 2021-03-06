#ifndef __INDEX_H__
#define __INDEX_H__

#define STARTING_IND_SIZE 600

#include <stdint.h>

typedef long NodeIndex;

NodeIndex* createNodeIndex();	// malloc + init

int insertNode(NodeIndex* index, uint32_t nodeId, long offset);

int getListHead(NodeIndex* index, uint32_t nodeId, int current_ind_size);	// return index[nodeId];

int destroyNodeIndex(NodeIndex* index);	// free

int double_index(NodeIndex **index, uint32_t nodeId, int current_ind_size, int *multiplier);

#endif
