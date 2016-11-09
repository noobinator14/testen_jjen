#ifndef __BUFFER_H__
#define __BUFFER_H__

#define STARTING_BUF_SIZE 100000		// how many structs fit
#define NEIGHB 100
#define NEIGHB_EXISTS -4
#define OK_SUCCESS -5
#define FAILURE -6

#include <stdint.h>

typedef struct {

	uint32_t neighbor[NEIGHB];
	//uint32_t edgeProperty[NEIGHB];
	long nextListNode;

} list_node ;

typedef struct {

	void *base_addr;
	unsigned long allocated;
	unsigned long max_size;

} Buffer ;


Buffer* createBuffer();	// malloc (+init) enan (poly) megalo xwro (pollaplasio tou sizeof(list_node) isws?)

int allocNewNode(Buffer* buf);	// an xwraei allo list_node [max_size-allocated>=sizeof(list_node)], return allocated
				// kai Buffer->allocated+=sizeof(list_node)

long add_neighbour(Buffer * buf, long off, uint32_t to);

void getListNode(void *addr, list_node *my_node);

int destroyBuffer(Buffer* buf);	// free

int double_buffer(Buffer **buf, int current_buf_size);



#endif
