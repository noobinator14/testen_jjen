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
		memmove((buf->base_addr)+(buf->allocated),&new,sizeof(list_node));
		ret=buf->allocated;
		buf->allocated+=sizeof(list_node);
	}
	return ret;
}

void getListNode(void *addr, list_node *my_node) {

	memmove(my_node,addr,sizeof(list_node));
}

long add_neighbour(Buffer * buf, long off, uint32_t to) {

	long i=0, added=0, writeback;

	list_node my_node;
	writeback=off;
	getListNode((buf->base_addr)+off,&my_node);
	while (added==0) {
		for (i=0;i<NEIGHB;i++) {
			if (my_node.neighbor[i]==to)
				return NEIGHB_EXISTS;
			if (my_node.neighbor[i]==-1) {
				my_node.neighbor[i]=to;
				memmove((buf->base_addr)+writeback,&my_node,sizeof(list_node));
				added=1;
				return OK_SUCCESS;
			}
		}
		if (added==0 && my_node.nextListNode!=-1) {
			writeback=my_node.nextListNode;
			getListNode((buf->base_addr)+writeback,&my_node);
		}
		else if (added==0 && my_node.nextListNode==-1)
			break;
	}
	return writeback;
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
