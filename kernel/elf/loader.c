#include "../include/elf.h"
#include "../include/mmu.h"
#include "../include/x86.h"
#include "../include/types.h"
#include "../include/memlayout.h"
#include "../include/stdio.h"
#include "../include/common.h"
#include "../include/string.h"
#include "../include/pmap.h"
#include "../include/irq.h"
#include "../include/trap.h"
#include "../include/fs.h"
#include "../include/disk.h"
#include "../include/env.h"
#include "../include/process/env.h"

#define SECTSIZE 512

//void readseg(unsigned char *, int, int);
extern struct PageInfo* page_free_list; 
unsigned char buffer[4096];

void* loader(struct Env *p_env,int diskoff)
{
	struct Elf *elf;
	struct Proghdr *ph, *eph;
	unsigned char pagebuffer[4096];

	elf = (struct Elf*)buffer;
	//readseg((unsigned char*)elf,4096,0);
	printk("Hello loader!\n");
	int fd = 0;
	fs_read(fd,(void *)elf,4096);
	fs_rewind(fd);
	printk("directory_d.entries[0] = %s\n",directory_d.entries[0].filename);
	printk("The magic number of elf = 0x%08x\n",elf->e_magic);
	printk("the entry of elf = 0x%08x\n",elf->e_entry);

	ph = (struct Proghdr*)((char*)elf + elf->e_phoff);
	eph = ph + elf->e_phnum;
	for (;ph < eph;ph++)
	{
		uint32_t va = ph->p_va;
		int data_loaded = 0;
		if (ph->p_type == 1)
		{
			while (va < ph->p_va + ph->p_memsz)
			{
				memset(pagebuffer,0,4096);
				uint32_t offset = va & 0xfff;
				va = va & 0xfffff000;
				struct PageInfo *page = page_alloc(1);
				page_insert(p_env->env_pgdir,page,(void *)va,PTE_U | PTE_W);
				int n = (4096 - offset) > (ph->p_filesz - data_loaded) ? (ph->p_filesz - data_loaded) : (4096 - offset);
				//readseg((unsigned char*)(pagebuffer + offset),n,ph->p_offset + data_loaded);
				if (n != 0)
				{
					fs_lseek(fd,ph->p_offset + data_loaded,SEEK_SET);
					fs_read(fd,(void*)(pagebuffer + offset),n);
					//readseg((unsigned char*)(pagebuffer + offset),n,ph->p_offset + data_loaded,diskoff);
				}
				memcpy((void *)page2kva(page),pagebuffer,4096);
				va += 4096;
				data_loaded += n;
			}	
		}
	}
	region_alloc(p_env,(void *)(USTACKTOP - 1024 * PGSIZE),1024 * PGSIZE);
	p_env->env_tf.eip = elf->e_entry;
	return (void*)elf->e_entry;
}

void waitdisk(void)
{
	while((inb(0x1F7) & 0xC0) != 0x40); 
}

void readsect(void *dst, int offset)
{
	int i;
	waitdisk();
	outb(0x1F2, 1);
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);

	waitdisk();
	for (i = 0; i < SECTSIZE / 4; i ++) 
		((int *)dst)[i] = in_long(0x1F0);
}

void writesect(void *src, int offset)
{
	int i;
	waitdisk();
	outb(0x1F2, 1);
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x30);
	waitdisk();
	for (i = 0; i < SECTSIZE / 4; i ++) 
		out_long(0x1F0,((unsigned int *)src)[i]);
}

void readseg(unsigned char *pa, int count, int offset,int diskoff)
{
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + diskoff + 1;
	for(; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}
