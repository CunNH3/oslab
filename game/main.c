#include "include/common.h"
#include "include/x86.h"
#include "include/video.h"
#include "include/string.h"
#include "include/game.h"
#include "include/keyboard.h"
#include "include/timer.h"
#include "include/system.h"
#include "include/stdio.h"

volatile int tick = 0;

void timer_event(void)
{
	tick++;
}

void init_game()
{
	syscall(0, timer_event);
	syscall(1, keyboard_event);
}

void testprintf();

void sleep(int time)
{
	int pretime = tick;
	while (tick - pretime < time * 100);
}
int game_main()
{
	init_game();
	testprintf();
	while (1)
	{
		printf("Game init!\n");
		clear_letter_pressed();
		draw_black_screen();
		prepare_buffer();
		draw_border();
		display();
		init_squares();
		printf("Game start!\n");
		game_loop();
		sleep(2);
	}
	return 0;
}

void fork_test()
{
	int i;
	for (i = 0;i < 2;i++)
	{
		printf("We start to test fork.\n");
		system_env_fork();
		printf("We fork a process to test.\n");
	}
	while(1);
}

void sleep_test()
{
	if (system_env_fork())
	{
		printf("We start to test sleep.\n");
		system_env_sleep(100);
		printf("The sleep test succeed.\n");
	}
	while(1);
	system_env_exit();
	printf("We exit this process.\n");
}

void exit_test()
{
	int i;
	for (i = 0;i < 2;i++)
	{
		system_env_fork();
		printf("We fork a process to test exit then.\n");
	}
	system_env_exit();
	while(1);
}

int fs_test()
{
	char poem[50][50];
	int i;
	printf("Let us read poem and continue 1 second!\n");
	int fp1 = file_open("readpoem.txt",0);
	printf("fp1 = %d\n",fp1);
	for (i = 0;i < 4;i++)
	{
		file_read(fp1,(void *)poem[i],50);
		printf("%s\n",poem[i]);
	}
	file_close(fp1);
	int fp2 = file_open("writepoem.txt",0);
	printf("fp2 = %d\n",fp2);
	for (i = 0;i < 4;i++)
		file_write(fp2,(void *)poem[i],50);
	memset(poem,0,sizeof(poem));
	file_rewind(fp2);
	for (i = 0;i < 4;i++)
	{
		file_read(fp2,(void *)poem[i],50);
		printf("%s\n",poem[i]);
	}
	file_close(fp2);
	printf("The poem has been read and written and we have continue 1s for him O_O!\n");
	while (1);
	return 0;
}

