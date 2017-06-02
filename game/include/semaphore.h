#include "env.h"
#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#define mutex 0
#define full 1
#define empty 2
#define slotN 10

typedef struct
{
	bool effective;
	bool binary;
	bool binary_num;
	int num;
	struct Env* wait_list;
} semaphore_t;

extern semaphore_t semaphore[];
void sem_init();
void  sem_open(int ,bool,int);
void sem_destroy(int);
int sem_wait(int);
void sem_post(int); 


#endif
