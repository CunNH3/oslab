#include "include/x86.h"
#include "include/elf.h"
#include "include/common.h"
#include "include/string.h"
#include "include/device/video.h"
#include "include/device/video_mode.h"

#define SECTSIZE 512

void readseg(unsigned char *, int, int);

void init_vmem_addr();

void init_serial();
void init_intr();
void init_timer();
void init_idt();
void add_irq_handle(int,void (*)(void));
void timer_event();
void keyboard_event();

void testprintk();
void serial_output_test();

void init()
{
	init_vmem_addr();
	init_serial();
	init_intr();
	init_idt();
	init_timer();
	init_vmem();

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
	init();test();
	printk("Hello, kernel!\n");

	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char *pa, *i;

	//elf = (struct ELFHeader*)0x190000;
	uint8_t buf[4096];
	elf = (struct ELFHeader*)buf;

	readseg((unsigned char*)elf, 4096, 10 * 1024 * 1024);


	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
	for(;ph < eph;ph++)
	{
		pa = (unsigned char*)ph->paddr; 
		readseg(pa, ph->filesz,10 * 1024 * 1024 + ph->off);
		for (i = pa + ph->filesz;i < pa + ph->memsz;*i ++ = 0);
	}
	printk("Start!\n");
	
	((void(*)(void))elf->entry)();
	while(1);
	return 0;
}

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
