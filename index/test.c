#include <stdio.h>
#include <stdint.h>
#include "../buffer/buffer.h"
#include "index.h"

int main() {

	int pos=0, new_pos=0, current_ind_size=0; 
	uint32_t num=0, num2=0;

	NodeIndex *index=createNodeIndex();
	printf("Index created with size %d\n",STARTING_IND_SIZE);
	current_ind_size=STARTING_IND_SIZE;
	num=15;
	printf("Let's see where is the start of node %d\n",num);
	pos=getListHead(index,num,current_ind_size);
	if (pos==-2)
		printf("%d is bigger than index size...\n",num);
	else if (pos==-1) {
		printf("Node %d not registered... Let's add it!\n",num);
		if (insertNode(index,num,666)==OK_SUCCESS)
			printf("Node %d added!\n",num);
		else
			printf("FAIL: node %d not added\n",num);
		new_pos=getListHead(index,num,current_ind_size);
		if (new_pos>=0)
			printf("Now, node %d is %d bytes away from the start\n",num,new_pos);
		else
			printf("Something went very wrong...\n");
	}
	else
		printf("Node %d is %d bytes away from the start\n",num,pos);
	num2=current_ind_size+9;
	printf("I want to add node %d\n",num2);
	pos=getListHead(index,num2,current_ind_size);
	if (pos>=0)
		printf("Node exists\n");
	else if (pos==-1) {
		if (insertNode(index,num2,665)==OK_SUCCESS)
			printf("Node %d added!\n",num2);
		else
			printf("FAIL: node %d not added\n",num2);
	}
	else {
		printf("Index is small... Let's double its size! :D \n");
		if (double_index(&index,current_ind_size)==OK_SUCCESS) {
			current_ind_size*=2;
			printf("Index doubled! New size=%d\n",current_ind_size);
		}
		else {
			perror("realloc");
			return -1;
		}
		new_pos=getListHead(index,num2,current_ind_size);
		if (new_pos>=0)
			printf("Node exists?!?!\n");
		else if (new_pos==-1) {
			if (insertNode(index,num2,665)==OK_SUCCESS)
				printf("Node %d added!\n",num2);
			else
				printf("FAIL: node %d not added\n",num2);
			new_pos=getListHead(index,num2,current_ind_size);
			if (new_pos>=0)
				printf("Now, node %d is %d bytes away from the start\n",num2,new_pos);
			else
				printf("Something went very wrong...\n");
		}
		else {
			printf("Pfff index isn't big enough... Doubling until %d fits in\n",num2);
			while (new_pos==-2) {
				if (double_index(&index,current_ind_size)==OK_SUCCESS) {
					current_ind_size*=2;
					printf("Index doubled! New size=%d\n",current_ind_size);
				}
				else {
					perror("realloc");
					return -1;
				}
				new_pos=getListHead(index,num2,current_ind_size);
			}
			if (new_pos==-1) {
				if (insertNode(index,num2,665)==OK_SUCCESS)
					printf("Finally!! Node %d added!\n",num2);
				else
					printf("FAIL: node %d not added\n",num2);
			}
			else
				printf("Something went very wrong...\n");
		}
	}
	printf("Let's get outta here! About to destroy index...\n");
	if (destroyNodeIndex(index)==OK_SUCCESS)
		printf("Done!\n");
	else {
		printf("FAIL: free index\n");
		return -1;
	}
	return 0;
}
