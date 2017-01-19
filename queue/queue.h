#ifndef __QUEUE__
#define __QUEUE__

#include "../job_sched/job_sched.h"

typedef struct TOuras * HandleOuras;
typedef struct kombos * kombosPtr;
typedef struct Job Job;

HandleOuras Oura_Constructor();
int Oura_Destructor(HandleOuras * oura);

int Oyra_keni(const HandleOuras OuraPtr);
int Oura_get_size(HandleOuras OuraPtr);

int Oyra_prosthiki(HandleOuras OuraPtr, Job *newJob);
Job * Oyra_apomakrynsh(HandleOuras OuraPtr);	// elegxos an einai kenh prin th klhsh ths apomakrunshs



#endif
