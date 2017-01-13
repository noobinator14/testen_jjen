#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../index/index.h"
#include "../buffer/buffer.h"
#include "../Bidirectional_BFS_SCC/bydir_SCC.h"
#include "../list/Pointer_List.h"
#include "../strongly_connected_comp/scc.h"
#include "grail.h"


GrailIndex * buildGrailIndex(NodeIndex* out_index,Buffer *out_buffer, int current_out_ind_size, scc* components) {
	int i,j;
	GrailIndex * index = malloc((components->comp_counts)*sizeof(GrailIndex));
	if (index == NULL) {
		printf("FAIL: malloc Grail\n");
		return NULL;
	}

	// post-order diasxush tou hypergraph

	for(i=0; i<components->comp_counts; i++) {
		index[i].nodesHY = malloc(components->comp[i].nodes_count*sizeof(uint32_t));
		if(index[i].nodesHY == NULL) {
			free(index);
			return NULL;
		}
		for(j=0; j<components->comp[i].nodes_count; j++) {
			index[i].nodesHY[j] = components->comp[j].node_id;
		}
		index[i].head = NULL;
		index[i].head = components->comp[i].head;
	}

	return index;
}

GRAIL_ANSWER isReachableGrailIndex(GrailIndex * index, scc *components, uint32_t source_node, uint32_t target_node) {
// vlepe ekfwnhsh sel. 9!!

	if ( components->id_belongs_to_comp[source_node] == components->id_belongs_to_comp[target_node] )
		return YES;
	else if (0)	//Polemikh kraugh
		return NO;
	else
		return MAYBE;
}

int destroyGrailIndex(GrailIndex* index, int comp_counts) {
	int i;
	if (index!=NULL) {
		for(i=0; i<comp_counts; i++) {
			if(index[i].nodesHY != NULL) {
				index[i].head = NULL;
				free(index[i].nodesHY);
			}
			else {
				printf("Master, this was not suppossed to happen. . .\n");
				printf("Master, what shall we do. . . ?\n");
			}
		}
		free(index);
		return OK_SUCCESS;
	}
	else	{
		//Einai adeio OG
		return FAILURE;
	}
}
