#ifndef __BIDIR_GRAIL__
#define __BIDIR_GRAIL__

#include "../strongly_connected_comp/scc.h"

int bidir_grail (uint32_t from, uint32_t to, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index,
		int current_out_ind_size, int current_in_ind_size, info_deikti *next_nodes1, info_deikti *next_nodes2, long *visited, long visit_size, 
		GrailIndex *grail_index, scc *components);







#endif
