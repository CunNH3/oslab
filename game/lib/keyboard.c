#include "../include/common.h"
#include "../include/x86.h"
#include "../include/scan_code.h"

#define NR_KEYS 6

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

static const int keycode_array[] = {K_UP, K_DOWN, K_LEFT, K_RIGHT, K_Z, K_ENTER};

static int key_state[NR_KEYS];

void get_press_key();
void get_release_key();
void enable_reborn();
void close_reborn();

void keyboard_event(void)
{
	int key_code = inb(0x60);
	int i;
	for (i = 0;i < NR_KEYS;i ++)
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

void process_keys()
{
	cli();
	int i;
	for (i = 0;i < NR_KEYS;i++)
	{
		switch(key_state[i])
		{
			case KEY_STATE_PRESS:
			{
				get_press_key(keycode_array[i]);
				key_state[i] = KEY_STATE_WAIT_RELEASE; break;
			}
			case KEY_STATE_RELEASE:
			{
				get_release_key(keycode_array[i]);
				key_state[i] = KEY_STATE_EMPTY; break;
			}
			default: break;
		}
	}
	sti();
}


extern int inc_player;
extern bool enable_bullet;

void get_press_key(uint32_t keycode)
{
	switch (keycode)
	{
		case K_LEFT: inc_player --; break;
		case K_RIGHT: inc_player ++; break;
		case K_Z: enable_bullet = true; break;
		case K_ENTER: enable_reborn(); break;
		case K_UP:
		case K_DOWN:
		default: break;
	}
}

void get_release_key(uint32_t keycode)
{
	switch (keycode)
	{
		case K_LEFT: inc_player ++; break;
		case K_RIGHT: inc_player --; break;
		case K_Z: enable_bullet = false; break;
		case K_ENTER: close_reborn(); break;
		case K_UP:
		case K_DOWN:
		default: break;
	}
}
