#include "../include/common.h"
#include "../include/string.h"


//enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

static int letter_code[] =
{
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};

static bool letter_pressed[26];

void press_key(int scan_code)
{
	int i;
	for (i = 0;i < 26;i++)
		if (letter_code[i] == scan_code) letter_pressed[i] = TRUE;
}

void release_key(int index)
{
	assert((0 <= index) && (index < 26));
	letter_pressed[index] = FALSE;
}

bool query_key(int index)
{
	assert((0 <= index) && (index < 26));
	return letter_pressed[index];
}

static volatile int key_code = 0;

int last_key_code(void)
{
	return key_code;
}

void keyboard_event(int code)
{
	key_code = code;
	press_key(code);
}

