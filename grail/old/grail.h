#ifndef __GRAIL__
#define _GRAIL__

#include "../strongly_connected_comp/scc.h"

typedef struct GrailIndex {
	//uint32_t node_count;
	uint32_t *nodesHY;	//Πίνακας με τους κόμβους που έχει κάθε HyperNode
	//uint32_t out_comps;
	neighb_comp * head;	//Λίστα με τα S.C.C. στο οποία μπορούμε να πάμε από το συγκεκριμένο S.C.C.
} GrailIndex;


typedef enum {NO=0, MAYBE=1, YES=2} GRAIL_ANSWER;



GrailIndex * buildGrailIndex(NodeIndex* out_index,Buffer *out_buffer, int current_out_ind_size, scc* components);

GRAIL_ANSWER isReachableGrailIndex(GrailIndex * index, scc *components, uint32_t source_node, uint32_t target_node);

int destroyGrailIndex(GrailIndex* index, int comp_counts);



#endif
