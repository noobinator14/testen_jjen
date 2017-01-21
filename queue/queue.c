#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct TOuras {
	kombosPtr arxi;
	kombosPtr telos;
	int metritis;		/* posa stoixeia uparxoun sthn oyra */
} TOuras;

typedef struct kombos {
	Job *data;
	kombosPtr epomenos;
} kombos;


HandleOuras Oura_Constructor() {
	HandleOuras oura=malloc(sizeof(TOuras));
	oura->arxi=NULL;
	oura->telos=NULL;
	oura->metritis=0;	// adeia oura
	return oura;
}

int Oura_Destructor(HandleOuras * oura) {
	kombosPtr current,next;

	if ((*oura)->metritis>0) {
		current=(*oura)->arxi;
		while (current!=NULL) {
			next=current->epomenos;
			free(current);
			current=next;
		}
	}
	free(*oura);
	*oura=NULL;
	return 0;
}

int Oyra_keni(const HandleOuras OuraPtr) {
	return (OuraPtr->metritis==0);
}

int Oura_get_size(HandleOuras OuraPtr) {
	return OuraPtr->metritis;
}

int Oyra_prosthiki(HandleOuras OuraPtr, Job *newJob) {
	kombosPtr node=malloc(sizeof(kombos));
	if (node==NULL) {
		printf("FAIL: malloc Oyra_prosthiki\n");
		return -1;
	}
	node->data=newJob;
	node->epomenos=NULL;
	if (OuraPtr->metritis==0) {
		OuraPtr->arxi=node;
		OuraPtr->telos=node;
	}
	else {
		(OuraPtr->telos)->epomenos=node;
		OuraPtr->telos=node;
	}
	OuraPtr->metritis++;
	return 0;
}

Job * Oyra_apomakrynsh(HandleOuras OuraPtr) {
// elegxos an einai kenh prin th klhsh ths apomakrunshs

	Job *ret=(OuraPtr->arxi)->data;
	kombosPtr tmp=(OuraPtr->arxi)->epomenos;
	free(OuraPtr->arxi);
	OuraPtr->arxi=tmp;
	OuraPtr->metritis--;
	if (OuraPtr->metritis==0)
		OuraPtr->telos=NULL;
	return ret;
}

