#include "../include/common.h"
#include "../include/x86.h"
#include "../include/scan_code.h"
#include "../include/syscall.h"

#define NR_KEYS 6

enum {STATE_EMPTY, STATE_WAIT_RELEASE, STATE_RELEASE, STATE_PRESS};

void get_press_key(uint8_t);
void get_release_key(uint8_t);
void enable_restart();
void close_restart();

bool keyboard_process()
{
	uint8_t keycode = get_keyboard();
	if(keycode == 0xff) return false;
	if(keycode < 0x80) 
	{
		get_press_key(keycode);
		return true;
	}
	else
	{
		get_release_key(keycode - 0x80);
		return true;
	}
}


extern int inc_player;
extern int des_player;
extern bool enable_bullet;

void get_press_key(uint8_t keycode)
{
	switch (keycode)
	{
		case K_A: inc_player--; break;
		case K_D: inc_player++; break;
		case K_SPACE: enable_bullet = true; break;
		case K_ENTER: enable_restart(); break;
		case K_W: des_player--;break;
		case K_S: des_player++;break;
		default: break;
	}
}

void get_release_key(uint8_t keycode)
{
	switch (keycode)
	{
		case K_A: inc_player++; break;
		case K_D: inc_player--; break;
		case K_SPACE: enable_bullet = false; break;
		case K_ENTER: close_restart(); break;
		case K_W:des_player--;break;
		case K_S:des_player++;break;
		default: break;
	}
}
