#ifndef __SYSCALL_H__
#define __SYSCALL_H__

int get_time();
int get_keyboard();
int put_video(uint8_t*);
int write(int, char*, int);

#endif 
