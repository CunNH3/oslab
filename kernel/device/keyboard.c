#include "../include/common.h"
#include "../include/x86.h"
#include "../include/device/scan_code.h"

#define NR_KEYS 6

enum {STATE_EMPTY, STATE_WAIT_RELEASE, STATE_RELEASE, STATE_PRESS};

static const int keycode_array[] =
{
	K_W, K_S, K_A, K_D, K_SPACE, K_ENTER
};

static int state[NR_KEYS];

void keyboard_event(void)
{
	int code = inb(0x60);
	printk("the keycode = 0x%x\n",code);
	int i;
	for (i = 0; i < NR_KEYS;i++)
	{
		if (code == keycode_array[i])
		{
			switch(state[i])
			{
				case STATE_EMPTY:
				case STATE_RELEASE:
				case STATE_PRESS: state[i] = STATE_PRESS; break;
				case STATE_WAIT_RELEASE: state[i] = STATE_WAIT_RELEASE; break;
				default: /*assert(0);*/ break;
			}
			break;
		}
		else 
		if(code == keycode_array[i] + 0x80)
		{
			state[i] = STATE_RELEASE;
			break;
		}
	}
}

int handle_keys()
{
	cli();
	int i;
	for(i = 0; i < NR_KEYS; ++i)
	{
		if(state[i] == STATE_PRESS)
		{
			state[i] = STATE_WAIT_RELEASE;
			sti(); 
			return keycode_array[i];
		}
		else
		if(state[i] == STATE_RELEASE)
		{
			state[i] = STATE_EMPTY;
			sti();
			return keycode_array[i] + 0x80;
		}
	}
	sti();
	return 0xff;
} 
