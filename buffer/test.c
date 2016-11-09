#include <stdio.h>
#include <string.h>	// memmove()
#include "buffer.h"

int main() {

	int offset=0, new=0, current_buf_size=0;
	long ret=0, ret2=0;
	list_node node;

	Buffer* buf=createBuffer();
	current_buf_size=STARTING_BUF_SIZE;
	printf("Buffer created with size %ld bytes\n",buf->max_size);
	printf("Currently allocated bytes: %ld\n",buf->allocated);
	printf("Let's allocate a node..!\n");
	offset=allocNewNode(buf);
	if (offset==-1) {
		printf("Not enough space for new node...\n");
		printf("Max size of buffer: %ld\n",buf->max_size);
		printf("Currently allocated: %ld\n",buf->allocated);
		printf("Size of node: %ld\n",sizeof(list_node));
	}
	else {
		printf("Success!\n");
		printf("Currently allocated bytes: %ld\n",buf->allocated);

		printf("Let's add neighbour 6\n");
		ret=add_neighbour(buf,offset,6);
		if (ret==OK_SUCCESS) {
			printf("Success!\n");
			printf("Trying to add him again...\n");
			if (add_neighbour(buf,offset,6)==NEIGHB_EXISTS)
				printf("Neighbour exists\n");
			else
				printf("Something went wrong...\n");
		}
		else
			printf("Something went wrong...\n");

		printf("Let's add neighbour 7\n");
		ret=add_neighbour(buf,offset,7);
		if (ret==OK_SUCCESS) {
			printf("Success!\n");
			printf("Trying to add him again...\n");
			if (add_neighbour(buf,offset,7)==NEIGHB_EXISTS)
				printf("Neighbour exists\n");
			else
				printf("Something went wrong...\n");
		}
		else if (ret==NEIGHB_EXISTS)
			printf("Neighbour exists\n");
		else {
			printf("No space for more neighbours...\n");
			printf("Got to allocate a new node, link with the previous one and try again\n");
			new=allocNewNode(buf);
			if (new!=-1) {
				getListNode(buf->base_addr + ret,&node);
				node.nextListNode=new;
				memmove(&node,buf->base_addr + ret,sizeof(list_node));
				ret2=add_neighbour(buf,new,7);
				if (ret2==OK_SUCCESS)
					printf("Success\n");
				else
					printf("Something went wrong...\n");
			}
			else {
				printf("No space for more nodes in this buffer... Got to double it!\n");
				if (double_buffer(&buf,current_buf_size)==OK_SUCCESS) {
					printf("Buffer doubled!\n");
					printf("Old buffer size: %d bytes\n",current_buf_size*sizeof(list_node));
					current_buf_size*=2;
					printf("New buffer size: %d bytes\n",current_buf_size*sizeof(list_node));
					printf("Currently allocated: %ld\n",buf->allocated);
					new=allocNewNode(buf);
					if (new!=-1) {
						getListNode(buf->base_addr + ret,&node);
						node.nextListNode=new;
						memmove(&node,buf->base_addr + ret,sizeof(list_node));
						ret2=add_neighbour(buf,new,7);
						if (ret2==OK_SUCCESS)
							printf("Success\n");
						else
							printf("Something went wrong...\n");
					}
					else
						printf("Something went wrong...\n");
				}
				else {
					perror("realloc");
					return -1;
				}
			}
		}
	}
	printf("About to destroy buffer...\n");
	if (destroyBuffer(buf)==OK_SUCCESS)
		printf("Done!\n");
	else {
		printf("FAIL: free buffer\n");
		return -1;
	}
	return 0;
}
