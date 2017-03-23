#include "game.h"
#include "common.h"
#include "string.h"
#include "adt/linklist.h"
#include "device/video.h"
#include "x86/x86.h"

LINKLIST_IMPL(fly,10000)

static fly_t head = NULL;
static int hit = 0, miss = 0;

int get_hit(void)
{
	return hit;
}

int get_miss(void) 
{
	return miss;
}

fly_t characters(void)
{
	return head;
}

void create_new_fly1()
{
	head->x = 0;
	head->y = rand() % (SCR_WIDTH / 8 - 2) * 8 + 8;
}

void create_new_fly2()
{
	head->x = rand() % (SCR_HEIGHT / 8 - 2) * 8 + 8;
	head->y = 0;
}
void create_new_letter(void)
{
	if (head == NULL)
		head = fly_new();
	else 
	{
		fly_t now = fly_new();
		fly_insert(NULL,head,now);
		head = now;
	}
	create_new_fly1();
	create_new_fly2();
	head->v = (rand() % 1000 / 1000.0 + 1) / 2.0;
	head->text = rand() % 26;
	release_key(head->text);
}

void update_letter_pos(void)
{
	fly_t it;
	for (it = head;it != NULL;)
	{
		fly_t next = it->_next;
		it->x += it->v;
		if ((it->x < 0) || (it->x + 7.9 > SCR_HEIGHT))
		{
			if (it->x < 0) hit ++; else miss ++;
			fly_remove(it);
			fly_free(it);
			if (it == head) head = next;
		}
		it = next;
	}
}

bool update_keypress(void)
{
	fly_t it,target = NULL;
	float min = -100;
	disable_interrupt();
	for (it = head;it != NULL;it = it->_next)
	{
		assert((it->text >= 0) && (it->text < 26));
		if ((it->v > 0) && (it->x > min) && (query_key(it->text)))
		{
			min = it->x;
			target = it;
		}
	}
	if (target != NULL)
	{
		release_key(target->text);
		target->v = -5;
		return TRUE;
	}
	enable_interrupt();
	return FALSE;
}

