#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

stoiva * Stoiva_dimiourgia(int size) {
	if (size<=0) {
		printf("Stack needs size > 0\n");
		return NULL;
	}
	stoiva *s=malloc(sizeof(stoiva));
	if (s==NULL) {
		printf("FAIL: malloc stack\n");
		return NULL;
	}
	s->korifi=-1;
	(s->pinakas)=malloc(size*sizeof(int));		// xreiazetai arxikopoihsh..?
	if (s->pinakas==NULL) {
		printf("FAIL: malloc stack.pinakas\n");
		return NULL;
	}
	s->size=size;
	return s;
}

void Stoiva_katastrofi(stoiva *s) {
	free(s->pinakas);
	free(s);
}

int Stoiva_keni(stoiva s) {
	return (s.korifi==-1);
}

int Stoiva_pop(stoiva *s) {
	if (!Stoiva_keni(*s)) {
		(s->korifi)--;			// na kanw thn kenh 8esh -1?
		return s->pinakas[s->korifi+1];
	}
	else
		return -1;		// stack empty
}

int Stoiva_push(stoiva *s, int elem) {
	if (s->korifi+1==s->size)	// stack full
		return -1;
	s->korifi++;
	s->pinakas[s->korifi]=elem;
	return 0;
}




