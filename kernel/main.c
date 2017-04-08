#include "include/x86.h"
#include "include/elf.h"
#include "include/stdio.h"
#include "include/const.h"
#include "include/common.h"
#include "include/string.h"
#include "include/device/video.h"
#include "include/video_mode.h"

#define SECTSIZE 512

void readseg(unsigned char *, int, int);

void init_intr();
void init_timer();
void init_serial();
void init_idt();
void init_vmem();
void init_vmem_addr();
void testprintk();

int kernel_main()
{
	init_vmem_addr();
	init_intr();
	init_timer();
	init_serial();
	init_idt();
	init_vmem();
	testprintk();

	printk("Hello, kernel!\n");

	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char* pa, *i;

	elf = (struct ELFHeader*)0x190000;


	readseg((unsigned char*)elf, 4096, 10 * 1024 * 1024);


	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
	for(;ph < eph;ph++)
	{
		pa = (unsigned char*)ph->paddr; 
		readseg(pa, ph->filesz, ph->off);
		for (i = pa + ph->filesz;i < pa + ph->memsz;*i ++ = 0);
	}
	printk("Start!\n");
	
	((void(*)(void))elf->entry)();
	
	//printk("Wrong!\n");
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
