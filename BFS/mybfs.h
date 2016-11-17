#ifndef __MYBFS_H__
#define __MYBFS_H__


#include "../list/Pointer_List.h"


int mybfs (uint32_t from, uint32_t to, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index,
		int current_out_ind_size, int current_in_ind_size, info_deikti *next_nodes);



#endif
