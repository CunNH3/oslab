#include "../include/system.h"
#include "../include/env.h"
#include "../include/process/env.h"

void sem_open(int index,bool binary,int value)
{
	semaphore[index].effective = true;
	if (binary) semaphore[index].binary_num = value;
	else semaphore[index].num = value;
}

void sem_destroy(int index)
{
	semaphore[index].effective = false;
}

int sem_wait(int index)
{
	if (!semaphore[index].binary)
	{
		int temp = semaphore[index].num;
		if (temp = 0)
		{
			curenv->env_status = ENV_NOT_RUNNABLE;
			curenv->env_link = semaphore[index].wait_list;
			semaphore[index].wait_list = curenv;
			struct Env* a = seek_next_runnable();
			env_run(a);
			sem_wait(index);
			return 0;
		}
		else
		{
			semaphore[index].num--;
			return temp;
		}
	}
	else
	{
		int temp = semaphore[index].binary_num;
		if (temp == 0)
		{
			curenv->env_status = ENV_RUNNABLE;
			curenv->env_link = semaphore[index].wait_list;
			semaphore[index].wait_list = curenv;
			struct Env* a = seek_next_runnable();
			env_run(a);
			return 0;
		}
		else
		{
			semaphore[index].binary_num = 0;
			return temp;
		}
	}
		
}

void sem_post(int index)
{
	if (!semaphore[index].binary)
	{
		semaphore[index].num++;
		if (!semaphore[index].wait_list) return;
		else
		{
			curenv->status = ENV_RUNNABLE;
			struct Env*a = semaphore[index].wait_list;
			wait_list = wait_list->env_link;
			a.env_status = ENV_RUNNABLE;
			env_run(a);
			return;
		}
	}
	else
	{
		semaphore[index].binary_num = 1;
		if (!semaphore[index].wait_list) return;
		else
		{
			curenv->status = ENV_RUNNABLE;
			struct Env*a = semaphore[index].wait_list;
			wait_list = wait_list->env_link;
			a.env_status = ENV_RUNNABLE;
			env_run(a);
			return;
		}
	}

}
