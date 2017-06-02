#include "include/system.h"
#include "include/semaphore.h"
#include "include/common.h"

int items[10];
int item_id;

void producer(void)
{
	int i;
	while (1)
	{
		sem_wait(empty);
		sem_wait(mutex);
		items[item_id++] = 1;
		printf("producer: ");
		for (i = 0;i < 9;i++)
			printf("%d ",items[i]);
		printf("%d\n",items[9]);
		sem_post(mutex);
		sem_post(full);
	}
}

void consumer(void)
{
	int i;
	while (1)
	{
		sem_wait(full);
		sem_wait(mutex);
		items[--item_id] = 0;
		printf("consumer: ");
		for (i = 0;i < 9;i++)
			printf("%d ",items[i]);
		printf("%d\n",items[9]);
		sem_post(mutex);
		sem_post(empty);
	} 
}

int test_main()
{
	int i;
	item_id = 0;
	sem_open(mutex,true,1);
	sem_open(empty,false,10);
	sem_open(full,false,0);
	for (i = 0;i < 10;i++) items[i] = 0;
	for (i = 0;i < 4;i++)
	{
		thread_create(producer);
		thread_create(consumer);
	}
	while(1);
}
