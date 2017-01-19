#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../buffer/buffer.h"
#include "job_sched.h"

void * thread_master(void *arg);
void * thread_worker(void *arg);

JobSched * initialize_scheduler(int exec_thr) {
	int i,err;
	
	JobSched *scheduler=malloc(sizeof(JobSched));
	if (scheduler==NULL) {
		printf("FAIL: malloc scheduler\n");
		return NULL;
	}
	scheduler->execution_threads=exec_thr;
	scheduler->tids=malloc(exec_thr*sizeof(pthread_t));
	if (scheduler->tids==NULL) {
		printf("FAIL: malloc scheduler->tids\n");
		return NULL;
	}
	/* Dhmiourgia adeias ouras */
	scheduler->oura=Oura_Constructor();
	/* Initialize oura_mutex */
	err=pthread_mutex_init(&(scheduler->oura_mtx),NULL);
	if (err!=0) {
		printf("FAIL: mutex init\n");
		return NULL;
	}
	/* Initialize cond_var 'oura_has_job' */
	pthread_cond_init(&(scheduler->oura_has_job),NULL);
	/* Spawn master thread (index:0 at scheduler->tids[]) */
	err=pthread_create(&(scheduler->tids[0]),NULL,thread_master,NULL);
	if (err!=0) {
		fprintf(stderr, "pthread_create (master thread): %s\n", strerror(err));
		return NULL;
	}
	/* Spawn worker threads */
	for (i=1;i<exec_thr;i++) {
		err=pthread_create(&(scheduler->tids[i]),NULL,thread_worker,NULL);
		if (err!=0) {
			fprintf(stderr, "pthread_create (worker thread): %s\n", strerror(err));
			return NULL;
		}
	}
	return scheduler;
}

int destroy_scheduler(JobSched *scheduler) {
	int err;
	Job *j;

	/* Join only master thread (master will join all workers) */
	err=pthread_join(scheduler->tids[0],NULL);
	if (err!=0) {
		fprintf(stderr, "pthread_join (master thread): %s\n", strerror(err));
		return -1;
	}
	/* Destroy oura_mutex */
	err=pthread_mutex_destroy(&(scheduler->oura_mtx));
	if (err!=0) {
		fprintf(stderr, "pthread_mutex_destroy: %s\n", strerror(err));
		return -1;
	}
	/* Destroy cond_var */
	err=pthread_cond_destroy(&(scheduler->oura_has_job));
	if (err!=0) {
		fprintf(stderr, "pthread_cond_destroy: %s\n", strerror(err));
		return -1;
	}
	free(scheduler->tids);
	/* Destroy-free all jobs */
	while (!Oyra_keni(scheduler->oura)) {
		j=Oyra_apomakrynsh(scheduler->oura);
		free(j);
	}
	Oura_Destructor(&(scheduler->oura));
	free(scheduler);
	return OK_SUCCESS;
}

int submit_job(JobSched *scheduler, Job *j) {

	if ((Oyra_prosthiki(scheduler->oura,j))==-1)
		return -1;
	else
		return 0;
}

Job * create_job(char mod,uint32_t n1,uint32_t n2) {
	Job *j=malloc(sizeof(Job));
	if (j==NULL) {
		printf("FAIL: malloc job\n");
		return NULL;
	}
	j->mode=mod;
	j->node1=n1;
	j->node2=n2;
	return j;
}






