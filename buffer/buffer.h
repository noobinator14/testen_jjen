#ifndef __BUFFER_H__
#define __BUFFER_H__

#define STARTING_BUF_SIZE 5000
#define NEIGHB 5

#define OK_SUCCESS -8
#define FAILURE -9

#include <stdint.h>

typedef struct {

	uint32_t neighbor[NEIGHB];
	//uint32_t edgeProperty[NEIGHB];
	long nextListNode;
	int nextEmptySlot;			// -1 -> no empty slot in this list_node
	long lastListNode;			/* -1 if this is the last list_node (note: 1st node indicates correctly to the last) */

} list_node ;

typedef struct {

	void *base_addr;
	unsigned long allocated;
	unsigned long max_size;

} Buffer ;


Buffer* createBuffer();

int allocNewNode(Buffer* buf);

long add_neighbour(Buffer * buf, long first_node_off, uint32_t to);

void getListNode(void *addr, list_node *my_node);

int destroyBuffer(Buffer* buf);

int double_buffer(Buffer **buf, int current_buf_size);



#endif


