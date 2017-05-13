#include "../include/common.h"
#include "../include/irq.h"
#include "../include/assert.h"
#include "../include/mmu.h"
#include "../include/x86.h"
#include "../include/types.h"

#define NR_IRQ_HANDLE 32
#define NR_HARD_INTR 16 /* At most 16 kinds of hardware interrupts. */

void do_syscall(TrapFrame* tf);
void timer_event(void);
void keyboard_event(uint32_t);
/* TrapFrame的定义在include/memory.h
 * 请仔细理解这段程序的含义，这些内容将在后续的实验中被反复使用。 */
void irq_handle(struct TrapFrame *tf)
{
	if (tf->irq == 0x80)
		do_syscall(tf);
	else 
	if (tf->irq < 1000)
	{
		if(tf->irq == -1)
		{
			printk("%s, %d: Unhandled exception!\n", __FUNCTION__, __LINE__);
		}
		else
		{
			printk("%s, %d: Unexpected exception #%d!\n", __FUNCTION__, __LINE__, tf->irq);
		}
		assert(0);
	}

	else 
	if (tf->irq == 1000)
	{
		timer_event();
	} 
	else if (tf->irq == 1001) {
		uint32_t code = inb(0x60);
		uint32_t val = inb(0x61);
		outb(0x61, val | 0x80);
		outb(0x61, val);
		printk("%s, %d: key code = %x\n", __FUNCTION__, __LINE__, code);
		keyboard_event(code);
	}
	else if(tf->irq== 1014){
		
	}
	else {
		assert(0);
	}
}
