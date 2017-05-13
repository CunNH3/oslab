#ifndef __GAME_H__
#define __GAME_H__

#include "keyboard.h"

void set_keyboard_intr_handler(void(*ptr)(int));
void keyboard_event();
void press_key(int scan_code);
void release_key(int ch);
bool query_key(int ch);
int last_key_code(void);


int rand(void);
void srand(int seed);

void init_squares();
void game_loop();
#endif
