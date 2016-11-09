#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "Pointer_List.h"


typedef struct info_node {
	int size;
	typos_deikti arxi;	/*deikths sto proto komvo tis listas*/
	typos_deikti telos;
} info_node;   	/* o komvos plhroforias ths listas */


typedef struct typos_komvou {
	int dedomena;
	typos_deikti epomenos;
} typos_komvou;	  /*o typos toy kombou ths listas*/




info_deikti LIST_dimiourgia() {
	info_deikti linfo=NULL;
	linfo = malloc(sizeof(info_node));
	if (linfo==NULL) {
		perror("malloc LIST_dimiourgia");
		return NULL;
	}
   	linfo->size = 0;
	linfo->arxi = NULL;
	linfo->telos=NULL;
	return linfo;
}

void LIST_katastrofi(info_deikti * linfo) {
	typos_deikti todel, todel2;
	todel= (*linfo)->arxi;
	while(todel!=NULL) {
		todel2=todel;
		todel=(todel)->epomenos;
		free(todel2);
	}
	(*linfo)->arxi = NULL;
	free(*linfo);
	(*linfo)=NULL;
}

int LIST_keni(const info_deikti  linfo) {
	return ( linfo->arxi == NULL );
}

int eisagogi_telos (info_deikti *linfo, int num) {

	typos_deikti prosorinos;
	prosorinos=malloc(sizeof(typos_komvou));
	if (prosorinos==NULL) {
		perror("malloc");
		return -1;
	}
	prosorinos->dedomena=num;
	prosorinos->epomenos=NULL;
	if ((*linfo)->size==0) {
		(*linfo)->arxi=prosorinos;
		(*linfo)->telos=prosorinos;
	}
	else if ((*linfo)->size==1) {
		((*linfo)->arxi)->epomenos=prosorinos;
		(*linfo)->telos=prosorinos;
	}
	else {
		((*linfo)->telos)->epomenos=prosorinos;
		(*linfo)->telos=prosorinos;
	}
	(*linfo)->size ++;
	return 0;
}

int eksagogi_arxi (info_deikti *linfo) {
	int ret;

	if ( (*linfo)->size==1) {
		ret=((*linfo)->arxi)->dedomena;
		free((*linfo)->arxi);
		(*linfo)->arxi=NULL;
		(*linfo)->telos=NULL;
	}
	else {
		ret=((*linfo)->arxi)->dedomena;
		typos_deikti temp=(*linfo)->arxi;
		(*linfo)->arxi=((*linfo)->arxi)->epomenos;
		free(temp);
		temp=NULL;
	}
	(*linfo)->size --;
	return ret;
}

int anazhthsh (info_deikti linfo, int num) {
/* Returns 1 if exists, else 0 */

	if (!LIST_keni(linfo)) {
		typos_deikti current=linfo->arxi;
		while (current!=NULL) {
			if (current->dedomena==num)
				return 1;
			else
				current=current->epomenos;
		}
	}
	return 0;
}




