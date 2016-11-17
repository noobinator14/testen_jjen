#ifndef __HASH_H__
#define __HASH_H__

#include <stdint.h>

#define HASH_SIZE 300
#define BUCKET_SIZE 3		// megalytero toy 0

typedef struct bucket * bucketPtr;

typedef struct bucket {
	int cell[BUCKET_SIZE];
	int next_empty;
	bucketPtr next_bucket;
	bucketPtr last_bucket;
} bucket;




int hash_func(uint32_t x);

bucketPtr create_hashtable();

int destroy_hash(bucketPtr hash);

void free_next(bucketPtr curr);

int check_add(bucketPtr hash, uint32_t to, int skip_check);


#endif
