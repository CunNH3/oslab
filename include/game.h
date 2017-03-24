#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "adt/linklist.h"

void init_serial();

void timer_event(void);
void keyboard_event(int scan_code);

void press_key(int scan_code);
void release_key(int ch);
bool query_key(int ch);
int last_key_code(void);

LINKLIST_DEF(fly)
	float x, y;
	int text;
	float v;
LINKLIST_DEF_FI(fly)

void main_loop(void);

void create_new_letter(void);
void update_letter_pos(void);
bool update_keypress(void);

int get_hit(void);
int get_miss(void);
int get_fps(void);
void set_fps(int fps);
fly_t characters(void);

void redraw_screen(void);

int rand(void);
void srand(int seed);

#endif
