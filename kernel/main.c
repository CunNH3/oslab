#include "include/x86.h"
#include "include/elf.h"
#include "include/common.h"
#include "include/string.h"
#include "include/mmu.h"
#include "include/process.h"
#include "include/memory.h"
#include "include/device/video.h"
#include "include/device/video_mode.h"
#include "include/disk.h"

#define SECTSIZE 512
#define GAME_OFFSET_IN_DISK (10 * 1024 * 1024)

SegMan *mm_malloc(uint32_t, uint32_t, uint32_t);
void set_tss_esp0(int);
void set_segment(Segdesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit);


void init_vmem_addr();
void init_serial();
void init_intr();
void init_timer();
void init_idt();
void init_segment();
void init_memory();
//void add_irq_handle(int,void (*)(void));

//void timer_event();
//void keyboard_event();

void testprintk();
void serial_output_test();

void init()
{
	init_segment();
	init_vmem_addr();
	init_serial();
	init_intr();
	init_idt();
	init_timer();
	init_vmem();
	init_process();
	init_memory();
	//add_irq_handle(0, timer_event);
	//add_irq_handle(1, keyboard_event);
}

void test()
{
	testprintk();
	serial_output_test();
	printk("\n");
}
int kernel_main()
{
	init();test();
	printk("Hello, kernel!\n");

	PCB *create = new_process();
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char *pa, *i;

	//elf = (struct ELFHeader*)0x190000;
	uint8_t buf[4096];
	elf = (struct ELFHeader*)buf;
	printk("the addr of the buf: 0x%x\n",(uint32_t)buf);
	
	readseg((unsigned char*)elf, 4096, GAME_OFFSET_IN_DISK);


	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
	/*for(;ph < eph;ph++)
	{
		pa = (unsigned char*)ph->paddr; 
		readseg(pa, ph->filesz,GAME_OFFSET_IN_DISK + ph->off);
		for (i = pa + ph->filesz;i < pa + ph->memsz;*i ++ = 0);
	}*/
	SegMan *tmp[3];
	int p_flag[2] = {0xa, 0x2};
	int cnt = -1,_vaddr;
	for (;ph < eph;ph ++)
	{
		if (ph->type != ELF_PROG_LOAD) continue;
		cnt ++;
		tmp[cnt] = mm_malloc(ph->vaddr, ph->memsz, p_flag[cnt]);
		_vaddr = ph->vaddr;
		pa = (unsigned char*)tmp[cnt]->base;
		readseg(pa, ph->filesz,GAME_OFFSET_IN_DISK + ph->off); 
		for (i = pa + ph->filesz; i < pa + ph->memsz; *i ++ = 0);
	}

	printk("Start!\n");
	
	//((void(*)(void))elf->entry)();

	uint32_t eflags = read_eflags();

	TrapFrame *tf = &create->tf;
	set_tss_esp0((int)create->kstack + KSTACK_SIZE);
	tf->gs = tf->fs = tf->es = tf->ds = SELECTOR_USER(tmp[SEG_USER_DATA]->gdt);
	tf->eax = 0; tf->ebx = 1; tf->ecx = 2; tf->edx = 3;
	
	tf->eflags = eflags | FL_IF;
	tf->eip = elf->entry;
	tf->cs = SELECTOR_USER(tmp[SEG_USER_CODE]->gdt);
	tf->ss = SELECTOR_USER(tmp[SEG_USER_DATA]->gdt);
	tf->esp = 0x10000000 - tmp[1]->base + _vaddr;

	asm volatile("movl %0, %%esp" : :"a"((int)tf));
	asm volatile("popa");
	asm volatile("addl %0, %%esp" : :"a"(8));

	asm volatile("mov 24(%esp), %eax\n\t"
		     "movl %eax, %ds\n\t"
 		     "movl %eax, %es\n\t"
		     "movl %eax, %fs\n\t"
	             "movl %eax, %gs\n\t");
	asm volatile("iret");
	//outw(0x8A00, 0x8A00);
	//outw(0x8A00, 0x8E00);
	while(1);
	return 0;
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
