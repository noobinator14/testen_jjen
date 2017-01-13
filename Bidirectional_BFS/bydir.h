#ifndef __BYDIR_H__
#define __BYDIR_H__


#include "../list/Pointer_List.h"

extern long GLOBAL_VISIT;

int mybfs (uint32_t from, uint32_t to, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index,
		int current_out_ind_size, int current_in_ind_size, info_deikti *next_nodes1, info_deikti *next_nodes2, long *visited, long visit_size);



#endif
