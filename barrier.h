#pragma once
#include <semaphore.h>

#define THREADS 24

struct Barrier{

	sem_t mutex;
	sem_t turnstile;
	sem_t turnstile2;
	int count;

};

	
void init(struct Barrier* barrier){
	sem_init(&barrier->mutex, 0, 1);
	sem_init(&barrier->turnstile, 0, 0);
	sem_init(&barrier->turnstile2, 0, 0);

	barrier->count=0;
}

void phase1(struct Barrier* barrier){
	sem_wait(&barrier->mutex);
	barrier->count++;
	if(barrier->count==THREADS){
		for(int i=0; i<THREADS; i++)
			sem_post(&barrier->turnstile);
		
	}
	sem_post(&barrier->mutex);
	sem_wait(&barrier->turnstile);
}

void phase2(struct Barrier* barrier){
	sem_wait(&barrier->mutex);
	barrier->count--;
	if(barrier->count==0){
		for(int i=0; i<THREADS; i++)
			sem_post(&barrier->turnstile2);
		
	}
	sem_post(&barrier->mutex);
	sem_wait(&barrier->turnstile2);

}
