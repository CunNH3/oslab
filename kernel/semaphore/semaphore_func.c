#include "../include/system.h"
#include "../include/env.h"
#include "../include/process/env.h"
#include "../include/stdio.h"
#include "../include/common.h"
#include "../include/semaphore.h"

void sem_open(int index,bool binary,int value)
{
	semaphore[index].effective = true;
	if (binary) semaphore[index].binary_num = (bool)value;
	else semaphore[index].num = value;
}

void sem_destroy(int index)
{
	semaphore[index].effective = false;
}

int sem_wait(int index)
{
	if (!semaphore[index].effective)
	{
		printk("The semaphore is ineffective!\n");
		return -1;
	}
	if (!semaphore[index].binary)
	{
		int temp = semaphore[index].num;
		if (temp == 0)
		{
			curenv->env_status = ENV_NOT_RUNNABLE;
			curenv->env_link = semaphore[index].wait_list;
			semaphore[index].wait_list = curenv;
			struct Env* a = seek_next_runnable();
			env_run(a);
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
			curenv->env_status = ENV_NOT_RUNNABLE;
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
	if (!semaphore[index].effective)
	{
		printk("The semaphore is ineffective!\n");
		return;
	}
	if (!semaphore[index].binary)
	{
		if (!semaphore[index].wait_list)
		{
			semaphore[index].num++;
			return;
		}
		else
		{
			curenv->env_status = ENV_RUNNABLE;
			struct Env*a = semaphore[index].wait_list;
			semaphore[index].wait_list = semaphore[index].wait_list->env_link;
			a->env_status = ENV_RUNNABLE;
			env_run(a);
			return;
		}
	}
	else
	{
		if (!semaphore[index].wait_list) 
		{
			semaphore[index].binary_num = 1;
			return;
		}
		else
		{
			curenv->env_status = ENV_RUNNABLE;
			struct Env*a = semaphore[index].wait_list;
			semaphore[index].wait_list = semaphore[index].wait_list->env_link;
			a->env_status = ENV_RUNNABLE;
			env_run(a);
			return;
		}
	}

}
