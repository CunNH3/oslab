#include "include/x86.h"
#include "include/elf.h"
#include "include/common.h"
#include "include/string.h"
#include "include/mmu.h"
#include "include/memory.h"
#include "include/device/video.h"
#include "include/device/video_mode.h"
#include "include/irq.h"
#include "include/pmap.h"

extern pde_t entry_pgdir[];
void init_cond();
void init_mem();
void* loader();
void add_irq_handle(int,void (*)(void));
void init_idt();
void init_intr();
void init_serial();
void init_timer();
void testprintk();
void timer_event();
void keyboard_event(); 
int kernel_main()
{
	printk("before page init\n");
	page_init();
	printk("after page init\n");
	init_cond();
	printk("after condition init\n");
	return 0;
}

void init_cond()
{
	init_idt();
	
	init_intr();
	init_serial();
	init_timer();
	init_mem();
	
	//add_irq_handle(0, timer_event);
	//add_irq_handle(1, keyboard_event);
	
	testprintk();
	//asm volatile("sti");
	//to store kernel pgd
	struct PageInfo *page = page_alloc(1);
	uint32_t cr3_game = page2pa(page);
	pde_t *pgdir_game = page2kva(page);
	memcpy(pgdir_game,entry_pgdir,4096);
	//int *p = (int *)0xa0000;
	//*p = 0;
	//while(1);
	void* eip = loader(pgdir_game);
	lcr3(cr3_game);
	printk("eip = %x\n",eip);
	((void(*)(void))eip)();
	printk("shouldn't reach here");

}
/*
void waitdisk(void)
{
	while((inb(0x1F7) & 0xC0) != 0x40); 
}

void readsect(void *dst, int offset)
{
	//int i;
	waitdisk();
	outb(0x1F2, 1);
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);

	waitdisk();
	insl(0x1F0, dst, SECTSIZE/4);
}


void readseg(unsigned char *pa, int count, int offset)
{
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + 1;
	for(; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}
*/
