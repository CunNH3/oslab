#include "include/types.h"
#include "include/x86.h"
#include "include/stdio.h"
#include "include/assert.h"

const static int letter_code[] =
{
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};


#define NR_KEYS (sizeof(letter_code)/sizeof(int))

static int letter_pressed[26];

bool query_key(int index)
{
	assert((0 <= index) && (index < 26));
	return letter_pressed[index];
}

void do_keyboard(int scan_code)
{
	int i;
	bool flag;
	if (scan_code & 0x80)
	{
		flag = false;
		scan_code = scan_code - 0x80;
	}
	else flag = true;
	for (i = 0;i < 26;i++)
	{
		if (letter_code[i] == scan_code)
		{
			if (!flag) letter_pressed[i] = false; else letter_pressed[i] = true;
		}
	}
}
