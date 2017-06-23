#include "include/x86.h"
#include "include/elf.h"
#include "include/common.h"
#include "include/string.h"
#include "include/mmu.h"
#include "include/memory.h"
#include "include/video.h"
#include "include/system.h"
#include "include/memlayout.h"
#include "include/process/env.h"
#include "include/irq.h"
#include "include/pmap.h"
#include "include/game.h"
#include "include/semaphore.h"
#include "include/disk.h"
#include "include/fs.h"
#include "include/env.h"
#include "include/disk.h"

extern pde_t entry_pgdir[];
void init_cond();
void init_segment();
void init_mem();
//void* loader();

int kernel_main()
{
	printk("Hello kernel!\n");
	page_init();
	init_cond();
	return 0;
}
extern struct PageInfo pages[];

void init_cond()
{
	
	init_segment();
	init_idt();
	init_intr();
	init_serial();
	init_timer();
	init_mem();
	sem_init();
	readsect((void *)directory_d.entries,201 + 256);
	printk("Filename = %s\n",directory_d.entries[0].filename);
	env_init();
	kernel_env.file[0].opened = true;
	kernel_env.file[0].offset = 0;
	curenv = &kernel_env;
	set_timer_intr_handler(kernel_timer_event);
	//asm volatile("cli");
	
	env_create(200,0,ENV_TYPE_USER);
	env_run(&envs[0]);
}
