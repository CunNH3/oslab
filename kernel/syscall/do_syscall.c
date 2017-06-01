#include "../include/video.h"
#include "../include/mmu.h"
#include "../include/system.h"
#include "../include/x86.h"
#include "../include/elf.h"
#include "../include/string.h"
#include "../include/timer.h"
#include "../include/keyboard.h"
#include "../include/game.h"
#include "../include/irq.h"
#include "../include/semaphore.h"
#ifndef SERIAL_PORT
#define SERIAL_PORT 0x3F8

static uint32_t *vmembase = (uint32_t*)VMEM_ADDR;

void do_syscall(TrapFrame* tf)
{
	switch (tf->eax)
	{
		case drawpixel:
		{
			int offset = tf->ebx + tf->ecx * SCR_WIDTH;
			uint32_t *position = vmembase + offset;
			(*position) = tf->edx;
		}
		break;
		case serialprint:
			while ((inb(SERIAL_PORT + 5) & 0x20) == 0);
			outb(SERIAL_PORT,tf->ebx);
			break;
		case drawpixeloff:
		{
			uint32_t *position = vmembase + tf->ebx;
			(*position) = tf->ecx;
		}
		break;
		case clearscreen:
			memset((void*)vmembase,tf->ebx,SCR_SIZE);
			break;
		
		case 0:
			set_timer_intr_handler((void*)tf->ebx);
			break;
		case 1:
			set_keyboard_intr_handler((void*)tf->ebx);
			break;
		
		case initserial:
			init_serial();
			break;
		case inittimer:
			init_timer();
			break;
		case enableinterrupt:
			asm volatile("sti");
			break;
		case disenableinterrupt:
			asm volatile("cli");
			break;
		case env_fork:
			system_env_fork();
			break;
		case env_sleep:
			printk("sleep!!\n");
			system_env_sleep((uint32_t)tf->ebx);
			break;
		case env_exit:
			system_env_exit();
			break;
		case semopen:
			sem_open((int)tf->ebx,(bool)tf->ecx,(int)tf->edx);
			break;
		case semclose:
			sem_close((int)tf->ebx);
			break;
		case semwait:
			sem_wait((int)tf->ebx);
			break;
 		case sempost:
			sem_post((int)tf->ebx);
			break;
 	}
}

#endif
