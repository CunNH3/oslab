#include "../include/common.h"

enum {SYS_time, SYS_keyboard, SYS_video};

int __attribute__((__noinline__))
syscall(int id, ...)
{
	int ret;
	int *args = &id;
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

int get_time()
{
	return syscall(SYS_time);
}

int get_keyboard()
{
	return syscall(SYS_keyboard);
}

int put_video(uint8_t *buffer)
{
	return syscall(SYS_video, buffer);
} 
