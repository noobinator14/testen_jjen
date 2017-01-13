#ifndef __STACK__ 
#define __STACK__


typedef struct {
	int korifi;
	int* pinakas;
	int size;
} stoiva;

 
stoiva * Stoiva_dimiourgia(int size);

void Stoiva_katastrofi(stoiva *s);

int Stoiva_keni(stoiva s);

int Stoiva_pop(stoiva *s);

int Stoiva_push(stoiva *s, int elem);




#endif
