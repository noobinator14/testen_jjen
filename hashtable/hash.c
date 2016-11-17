#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "hash.h"


int hash_func(uint32_t x) {

	return (x % HASH_SIZE) ;
}

bucketPtr create_hashtable() {

	int i;

	bucketPtr hash=malloc(HASH_SIZE*sizeof(struct bucket));
	if (hash==NULL) {
		perror("malloc at create");
		return NULL;
	}
	for (i=0;i<HASH_SIZE;i++) {
		hash[i].next_empty=0;
		hash[i].next_bucket=NULL;
		hash[i].last_bucket=&hash[i];
	}
	return hash;
}

int destroy_hash(bucketPtr hash) {

	int i;
	bucketPtr curr=NULL;

	for (i=0;i<HASH_SIZE;i++) {
		curr=&hash[i];
		if (curr->next_bucket!=NULL)
			free_next(curr->next_bucket);
	}
	free(hash);
	return 0;
}

void free_next(bucketPtr curr) {

	if (curr->next_bucket!=NULL)
		free_next(curr->next_bucket);
	free(curr);
}

int check_add(bucketPtr hash, uint32_t to, int skip_check) {
/* Return 1 : neighbour existed , Return 0 : neighbour added , Return -1 : error */

	int i, last_cell=0;
	//int hash_value=hash_func(to);
	bucketPtr curr=hash;

	last_cell=hash->next_empty;
	if (skip_check==0) {
		/* Checking */
		for (i=0;i<last_cell;i++) {
			if ( hash->cell[i] == to )
				return 1;
		}
		if ( hash->next_bucket != NULL) {
			curr=hash->next_bucket;
			while (curr!=NULL) {
				last_cell=curr->next_empty;
				for (i=0;i<last_cell;i++) {
					if ( curr->cell[i] == to )
						return 1;
				}
				curr=(curr->next_bucket);
			}
		}
	}
	bucketPtr last=hash->last_bucket;
	last_cell=last->next_empty;
		/* Adding */
	if (last_cell==BUCKET_SIZE) {
		bucketPtr new=malloc(sizeof(struct bucket));
		if (new==NULL) {
			perror("malloc new");
			return -1;
		}
		new->cell[0]=to;
		new->next_empty=1;
		new->next_bucket=NULL;
		last->next_bucket=new;
		hash->last_bucket=last;		// 1st bucket knows who is the last bucket
		return 0;
	}
	//printf("sasa4 last_cell=%d\n",last_cell);
	last->cell[last_cell]=to;
	//printf("sasa5\n");
	(last->next_empty)++;
	return 0;
}






