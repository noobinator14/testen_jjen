#include <stdio.h>
#include "Pointer_List.h"

int main() {

	int elem=0;

	info_deikti list=LIST_dimiourgia();
	printf("A list has been created!\n");
	printf("Is list empty? ");
	if (LIST_keni(list))
		printf("Yes.\n");
	else
		printf("No.\n");
	printf("Is 7 in the list? ");
	if (anazhthsh (list,7))
		printf("Yes.\n");
	else {
		printf("No... Let's add it!\n");
		if (eisagogi_telos(&list,7)==0)
			printf("Added!\n");
		else
			printf("malloc error...\n");
	}
	printf("Is 4 in the list? ");
	if (anazhthsh (list,4))
		printf("Yes.\n");
	else {
		printf("No... Let's add it!\n");
		if (eisagogi_telos(&list,4)==0)
			printf("Added!\n");
		else
			printf("malloc error...\n");
	}
	if (!LIST_keni(list)) {
		printf("Let's pull the 1st element from the list\n");
		elem=eksagogi_arxi(&list);
		printf("Element=%d\n",elem);
	}
	printf("I want to search the list for number %d.\n",elem);
	if (anazhthsh (list,elem))
		printf("Found it!\n");
	else
		printf("Nothing found\n");
	printf("Let's destroy the list...\n");
	LIST_katastrofi(&list);
	printf("Done!\n");
	return 0;
}
