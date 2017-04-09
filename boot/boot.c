#include "include/x86.h"
#include "include/elf.h"

#define SECTSIZE 512
#define KERNEL_OFFSET_IN_DISK 0

void readseg(unsigned char*,int,int);

int bootmain(void)
{
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char *pa, *i;

	elf = (struct ELFHeader*)0x8000;

	readseg((unsigned char*)elf, 4096, KERNEL_OFFSET_IN_DISK);

	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
	for(; ph < eph; ph ++)
	{
		pa = (unsigned char*)ph->paddr;
		readseg(pa, ph->filesz,KERNEL_OFFSET_IN_DISK + ph->off);
		for(i = pa + ph->filesz; i < pa + ph->memsz; *i ++ = 0);
	}

	((void(*)(void))elf->entry)();

	while(1);
}

void waitdisk(void)
{
	while((inb(0x1f7) & 0xc0) != 0x40);
}

void readsect(void *dst, int offset)
{

	waitdisk();

	outb(0x1f2, 1);
	outb(0x1f3, offset);
	outb(0x1f4, offset >> 8);
	outb(0x1f5, offset >> 16);
	outb(0x1f6, (offset >> 24) | 0xe0);
	outb(0x1f7, 0x20);

	waitdisk();

	insl(0x1f0, dst, SECTSIZE/4);
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
