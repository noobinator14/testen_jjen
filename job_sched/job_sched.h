#ifndef __JOB_SCHED__
#define __JOB_SCHED__

#include <stdint.h>
#include "../queue/queue.h"
typedef struct TOuras * HandleOuras;


typedef struct JobSched {
	int execution_threads;
	pthread_t *tids;
	HandleOuras oura;
	pthread_mutex_t oura_mtx;
	pthread_cond_t oura_has_job;
} JobSched;

typedef struct Job {
	char mode;	// 'A', 'Q'
	uint32_t node1;
	uint32_t node2;
} Job;


JobSched * initialize_scheduler(int exec_thr);
int destroy_scheduler(JobSched *scheduler);
int submit_job(JobSched *scheduler, Job *j);



Job * create_job(char mod,uint32_t n1,uint32_t n2);

#endif
