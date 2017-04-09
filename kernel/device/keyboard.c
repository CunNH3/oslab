#include "../include/common.h"
#include "../include/x86.h"
#include "../include/device/scan_code.h"

#define NR_KEYS 6

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

static const int keycode_array[] =
{
	K_UP, K_DOWN, K_LEFT, K_RIGHT, K_Z, K_ENTER
};

static int key_state[NR_KEYS];

void keyboard_event(void)
{
	int key_code = inb(0x60);
	printk("the keycode = 0x%x\n",key_code);
	int i;
	for (i = 0; i < NR_KEYS;i++)
	{
		if (key_code == keycode_array[i])
		{
			switch(key_state[i])
			{
				case KEY_STATE_EMPTY:
				case KEY_STATE_RELEASE:
				case KEY_STATE_PRESS: key_state[i] = KEY_STATE_PRESS; break;
				case KEY_STATE_WAIT_RELEASE: key_state[i] = KEY_STATE_WAIT_RELEASE; break;
				default: /*assert(0);*/ break;
			}
			break;
		}
		else 
		if(key_code == keycode_array[i] + 0x80)
		{
			key_state[i] = KEY_STATE_RELEASE;
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
		if(key_state[i] == KEY_STATE_PRESS)
		{
			key_state[i] = KEY_STATE_WAIT_RELEASE;
			sti(); 
			return keycode_array[i];
		}
		else
		if(key_state[i] == KEY_STATE_RELEASE)
		{
			key_state[i] = KEY_STATE_EMPTY;
			sti();
			return keycode_array[i] + 0x80;
		}
	}
	sti();
	return 0xff;
} 
