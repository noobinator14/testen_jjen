#ifndef __SCC_H__
#define __SCC_H__

#include <stdint.h>
#include "../buffer/buffer.h"
#include "../index/index.h"
#include "../list/Pointer_List.h"


//STRUCTS

typedef struct neighb_comp {
	uint32_t next_SCC;
	struct neighb_comp * next;
} neighb_comp;

typedef struct infoSCC {
	uint32_t lowLink;
	uint32_t rank;
	int onS;
} infoSCC;

typedef struct comp {		//Single S.C.C.
	uint32_t out_comps;	//Number of neighbor components
	uint32_t component_id;	//id of this, specific, S.C.C.
	uint32_t nodes_count;	//The total number of nodes on this S.C.C.
	uint32_t * node_id;	//Array of the id of nodes inside this S.C.C.
	neighb_comp * head;	//head of the list containing all neighbor S.C.C.
	neighb_comp * nc;	//Pointer that helps us create the list, or access every member of it
} comp;

typedef struct scc {			//General Struct used
	comp * comp;			//An array of all of S.C.C.
	infoSCC *inS;
	uint32_t comp_counts;		//The total number of S.C.C. 
	uint32_t *id_belongs_to_comp;	//Matrix of every node in the graph. Each position contains the S.C.C. that node i is into
} scc; 

typedef struct ComponentCursor {	//Ένας Θεός ξέρει γιατί χρειάζεται αυτή η δομή... Με μία μεταβλητή είμασταν ΤζιΤζι, αλλά όχι Κων/να... Να μας πεθάνεις!
	comp * compt_ptr_cur;		//Current component that we check		
	comp *comp_ptr_prev;		//+ ό,τι άλλο χρειάζεται
	
} ComponentCursor;


//FUNCTIONS

scc * estimatedStronglyConnectComp(Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index, int current_out_ind_size, int current_in_ind_size, uint32_t bNode);

int destroyStronglyConnectedComponents(scc * SCC);


int findNodeStronglyConnectedComponetID(scc * compts, uint32_t nodeId);

/*

OK_SUCCESS iterateStronglyConnectComponentID(scc * comps,  ComponentCursor * cursor);

bool nextStronglyConnectedComponentID(scc * compts, ComponentCursor * cursor);

*/

int estimateShortestPathStronglyConnectedComponents(scc * compts, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index, int current_out_ind_size, int current_in_ind_size, uint32_t source_node, uint32_t target_node, info_deikti *next_nodes1, info_deikti *next_nodes2);

#endif
