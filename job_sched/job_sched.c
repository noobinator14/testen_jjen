#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../buffer/buffer.h"
#include "job_sched.h"

JobSched * initialize_scheduler(int exec_thr) {
	int err;
	
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
	/* Initialize CCindex_mutex */
	err=pthread_mutex_init(&(scheduler->CCindex_mtx),NULL);
	if (err!=0) {
		printf("FAIL: mutex init\n");
		return NULL;
	}
	/* Initialize grail_mutex */
	err=pthread_mutex_init(&(scheduler->grail_mtx),NULL);
	if (err!=0) {
		printf("FAIL: mutex init\n");
		return NULL;
	}
	/* Initialize file1_compl_mtx */
	err=pthread_mutex_init(&(scheduler->file1_compl_mtx),NULL);
	if (err!=0) {
		printf("FAIL: mutex init\n");
		return NULL;
	}
	/* Initialize addition_mtx */
	err=pthread_mutex_init(&(scheduler->addition_mtx),NULL);
	if (err!=0) {
		printf("FAIL: mutex init\n");
		return NULL;
	}
	/* Initialize cond_var 'file1_compl_cond' */
	pthread_cond_init(&(scheduler->file1_compl_cond),NULL);
	return scheduler;
}

int destroy_scheduler(JobSched *scheduler) {
	int err;
	Job *j;

	/* Destroy oura_mutex */
	err=pthread_mutex_destroy(&(scheduler->oura_mtx));
	if (err!=0) {
		fprintf(stderr, "pthread_mutex_destroy (oura_mtx): %s\n", strerror(err));
		return -1;
	}
	/* Destroy CCindex_mutex */
	err=pthread_mutex_destroy(&(scheduler->CCindex_mtx));
	if (err!=0) {
		fprintf(stderr, "pthread_mutex_destroy (CCindex_mtx): %s\n", strerror(err));
		return -1;
	}
	/* Destroy grail_mutex */
	err=pthread_mutex_destroy(&(scheduler->grail_mtx));
	if (err!=0) {
		fprintf(stderr, "pthread_mutex_destroy (grail_mtx): %s\n", strerror(err));
		return -1;
	}
	/* Destroy file1_compl_mtx */
	err=pthread_mutex_destroy(&(scheduler->file1_compl_mtx));
	if (err!=0) {
		fprintf(stderr, "pthread_mutex_destroy (file1_compl_mtx): %s\n", strerror(err));
		return -1;
	}
	/* Destroy addition_mtx */
	err=pthread_mutex_destroy(&(scheduler->addition_mtx));
	if (err!=0) {
		fprintf(stderr, "pthread_mutex_destroy (addition_mtx): %s\n", strerror(err));
		return -1;
	}
	/* Destroy 'file1_compl_cond' cond_var */
	err=pthread_cond_destroy(&(scheduler->file1_compl_cond));
	if (err!=0) {
		fprintf(stderr, "pthread_cond_destroy (file1_compl_cond): %s\n", strerror(err));
		return -1;
	}
	/* Destroy cond_var */
	err=pthread_cond_destroy(&(scheduler->oura_has_job));
	if (err!=0) {
		fprintf(stderr, "pthread_cond_destroy (oura_has_job): %s\n", strerror(err));
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






