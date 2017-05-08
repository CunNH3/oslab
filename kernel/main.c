#include "include/x86.h"
#include "include/elf.h"
#include "include/common.h"
#include "include/string.h"
#include "include/mmu.h"
#include "include/memory.h"
#include "include/device/video.h"
#include "include/device/video_mode.h"
#include "include/irq.h"
#define SECTSIZE 512
#define GAME_OFFSET_IN_DISK (10 * 1024 * 1024)
void readseg(unsigned char*,int,int);

void init_page();
void init_segment();
void init_vmem_addr();
void init_serial();
void init_intr();
void init_timer();
void init_idt();
void init_segment();
void init_memory();
void add_irq_handle(int,void (*)(void));
void set_trapframe(TrapFrame*, uint32_t);
void timer_event();
void keyboard_event();
void testprintk();
void serial_output_test();
int kernel_main(void);
void init()
{
	init_page();
	asm volatile("addl %0, %% esp" : : "i"(KOFFSET));
	asm volatile("jmp *%0" : : "r"(kernel_main));
	panic("Your page is fail!\n");
}
void init_kernel()
{
	init_segment();

	init_vmem_addr();
	init_vmem();

	init_pcb();
	init_pte_info();

	init_serial();
	init_intr();
	init_idt();
	init_timer();
	

	add_irq_handle(0, timer_event);
	add_irq_handle(1, keyboard_event);
}

void test()
{
	testprintk();
	serial_output_test();
	printk("\n");
}
int kernel_main()
{
	init_kernel();test();
	printk("Hello, kernel!\n");

	PCB *pcb_p = create_process(GAME_OFFSET_IN_DISK);
	set_trapframe((void*)pcb_p->kstack, pcb_p->entry);

	printk("Here we go!\n");

	asm volatile("movl %0, %%esp" : :"a"(pcb_p->kstack));
	asm volatile("popal;\
				  pushl %eax;\
				  movw 4(%esp), %ax;\
				  movw %ax, %gs;\
				  movw %ax, %fs;\
				  movw %ax, %es;\
				  movw %ax, %ds;\
				  popl %eax;\
				  addl $0x18, %esp;\
				  iret");
	panic("You are fail!\n");
	while (1);
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
