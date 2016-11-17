#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "buffer.h"

Buffer* createBuffer() {

	Buffer *buf=malloc(sizeof(Buffer));
	buf->base_addr=malloc(STARTING_BUF_SIZE*sizeof(list_node));
	buf->allocated=0;
	buf->max_size=STARTING_BUF_SIZE*sizeof(list_node);
	return buf;
}

int destroyBuffer(Buffer* buf) {

	free(buf->base_addr);
	buf->base_addr=NULL;
	free(buf);
	buf=NULL;
	return OK_SUCCESS;
}

int allocNewNode(Buffer* buf) {

	int i=0, ret=-1;

	if ( (buf->max_size) - (buf->allocated) < sizeof(list_node) )		// not enough space for new node
		return ret;
	else {
		list_node new;
		for (i=0;i<NEIGHB;i++) {
			new.neighbor[i]=-1;
			//new.edgeProperty[i]=-1;
		}
		new.nextListNode=-1;
		new.nextEmptySlot=0;
		new.lastListNode=-1;
		memmove((buf->base_addr)+(buf->allocated),&new,sizeof(list_node));
		ret=buf->allocated;
		buf->allocated+=sizeof(list_node);
	}
	return ret;
}

void getListNode(void *addr, list_node *my_node) {

	memmove(my_node,addr,sizeof(list_node));
}

long add_neighbour(Buffer * buf,long first_node_off, uint32_t to) {

	list_node nod,first;
	//printf("1st node offset = %ld\n",first_node_off);
	getListNode((buf->base_addr)+first_node_off,&first);		// get 1st node
	//printf("first.lastListNode = %ld\n",first.lastListNode);
	if (first.lastListNode==-1) {				// 1st is the last
		if (first.nextEmptySlot==-1)
			return first_node_off;
		else {
			first.neighbor[first.nextEmptySlot]=to;
			first.nextEmptySlot++;
			if (first.nextEmptySlot==NEIGHB)
					first.nextEmptySlot=-1;
			memmove((buf->base_addr)+first_node_off,&first,sizeof(list_node));
			return OK_SUCCESS;
		}
	}
	else {
		getListNode((buf->base_addr)+first.lastListNode,&nod);		// get last from 1st
		if (nod.lastListNode==-1) {			// this is the last
			if (nod.nextEmptySlot==-1) {		// no space for more neighbours here
				return first.lastListNode;			// returns offset of last node
			}
			else {
				nod.neighbor[nod.nextEmptySlot]=to;
				nod.nextEmptySlot++;
				if (nod.nextEmptySlot==NEIGHB)
					nod.nextEmptySlot=-1;
				memmove((buf->base_addr)+first.lastListNode,&nod,sizeof(list_node));
				return OK_SUCCESS;
			}
		}
		else
			return FAILURE;
	}
}

int double_buffer(Buffer **buf, int current_buf_size) {

	void *tmp=realloc((*buf)->base_addr,2*current_buf_size*sizeof(list_node));
	if (tmp!=NULL)
		(*buf)->base_addr=tmp;
	else
		return FAILURE;
	(*buf)->max_size=2*current_buf_size*sizeof(list_node);
	return OK_SUCCESS;
}


