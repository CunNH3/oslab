/*
#include "../include/x86.h"
#include "../include/game.h"
#include "../include/stdio.h"
#include "irq.h"

void do_timer(void);
void do_keyboard(int);

void do_syscall(TrapFrame *);

void set_timer_intr_handler(void (*ptr)(void))
{
	do_timer = ptr;
}

void set_keyboard_intr_handler(void (*ptr)(int))
{
	do_keyboard = ptr;
}

void irq_handle(struct TrapFrame *tf)
{
	if (tf->irq == 0x80) do_syscall(tf);
	else
	if(tf->irq < 1000)
	{
		if(tf->irq == -1)
			printk("%s, %d: Unhandled exception!\n", __FUNCTION__, __LINE__);
		else
			printk("%s, %d: Unexpected exception #%d!\n", __FUNCTION__, __LINE__, tf->irq);
		assert(0);
	}
	else
	if (tf->irq == 1000)
		timer_event();
	else
	if (tf->irq == 1001)
	{
		uint32_t code = inb(0x60);
		uint32_t val = inb(0x61);
		outb(0x61, val | 0x80);
		outb(0x61, val);
		printk("%s, %d: key code = %x\n", __FUNCTION__, __LINE__, code);
		do_keyboard(code);
	}
	else
		assert(0);
}
*/
#include "../include/common.h"
#include "irq.h"

#define NR_IRQ_HANDLE 32
#define NR_HARD_INTR 16

struct IRQ_t
{
	void (*routine)(void);
	struct IRQ_t *next;
};

static struct IRQ_t handle_pool[NR_IRQ_HANDLE];
static struct IRQ_t *handles[NR_HARD_INTR];
static int handle_count = 0;

void do_syscall(TrapFrame *);

void add_irq_handle(int irq, void (*func)(void))
{
	assert(irq < NR_HARD_INTR);
	assert(handle_count <= NR_IRQ_HANDLE);

	struct IRQ_t *ptr;
	ptr = &handle_pool[handle_count ++];
	ptr->routine = func;
	ptr->next = handles[irq];
	handles[irq] = ptr;
}

void irq_handle(TrapFrame *tf)
{

	int irq = tf->irq;

	if(irq == 0x80) do_syscall(tf);
	else 
	//if(irq < 1000) {printk("Unhandled irq = %d!\n",irq);assert(0);}
	//else
	{
		int irq_id = irq - 1000;
		assert(irq_id < NR_HARD_INTR);
		struct IRQ_t *f = handles[irq_id];

		while(f != NULL)
		{
			f->routine();
			f = f->next;
		}
	}
}

