#ifndef __MYBFS_H__
#define __MYBFS_H__


#include "../list/Pointer_List.h"
#include "../connected_comp/connected_comp.h"

int bfs_explore (CC *components, uint32_t start, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index, int current_out_ind_size,
			int current_in_ind_size, char *visited, int currentCC);



#endif
