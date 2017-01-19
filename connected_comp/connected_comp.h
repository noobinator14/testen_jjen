#ifndef __CONN_COMP_H__
#define __CONN_COMP_H__

#include <stdint.h>
#include "../index/index.h"
#include "../list/Pointer_List.h"


typedef struct {
	uint32_t *ccindex;
	info_deikti *updateIndex;
	int *priorityIndex;
	int ccindexSize;
	int updateIndexSize;
	int priorityIndexSize;
	int next_CC;
	float metric;
} CC ;


int CC_init(CC *components, int ccindex_size);

int destroyConnectedComponents(CC *components);

CC * estimateConnectedComponents(NodeIndex *out_index, NodeIndex *in_index, Buffer *out_buffer, Buffer *in_buffer, int curr_out_ind_size, int curr_in_ind_size);

int findNodeCCid(CC *components, uint32_t nodeId);

int insertNewEdge(CC *components, uint32_t nodeIdS, uint32_t nodeIdE);

int realloc_CCindex(CC *components, int multiplier);

int realloc_updateIndex(CC *components, int multiplier);

int realloc_priorityIndex(CC *components, int multiplier, int new_size);

int CC_search_for_connection(CC *components, uint32_t node1, uint32_t node2, int *queries_updateIndex);

int rebuildIndexes(CC *components);

#endif
