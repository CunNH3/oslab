#include "../include/common.h"
#include "../include/x86.h"
#include "../include/keyboard.h"
#include "../include/video.h"
#include "../include/pic.h"
#include "../include/string.h"

#define NR_ENEMY 10
#define NR_BULLET 100

extern uint8_t v_buffer[SCR_SIZE];
int x_player, inc_player;
bool enable_bullet;

void srand(uint32_t);
uint32_t rand();


struct Enemy
{
	int x, y;
	int w, h;
	struct Enemy *next;
} *enemy_head, *enemy_avail, enemy[NR_ENEMY];


bool generate_enemy()
{
	if(enemy_avail)
	{
		struct Enemy *temp = enemy_avail;
		enemy_avail = enemy_avail->next;
		temp->x = rand()%(SCR_WIDTH - FF_WIDTH); temp->y = 0;
		temp->w = FF_WIDTH; temp->h = FF_HEIGHT;
		temp->next = enemy_head;
		enemy_head = temp;
		return true;
	}
	else return false;
}

void delete_enemy(struct Enemy *enemy_p)
{
	struct Enemy *temp = enemy_head;
	if(temp == enemy_p)
	{
		enemy_head = enemy_p->next;
		enemy_p->next = enemy_avail;
		enemy_avail = enemy_p;
	}
	else
	{
		while(temp->next != enemy_p) temp = temp->next;
		temp->next = enemy_p->next;
		enemy_p->next = enemy_avail;
		enemy_avail = enemy_p;
	}
}

bool update_enemy()
{
	struct Enemy *temp = enemy_head;
	bool kill_flag;
	while(temp)
	{
		kill_flag = false;
		temp->y ++;
		if(temp->y >= SCR_HEIGHT - FF_HEIGHT) kill_flag = true;
		if((temp->y > SCR_HEIGHT - FF_HEIGHT - LS_HEIGHT) && (temp->x - x_player < LS_WIDTH && temp->x - x_player > -LS_WIDTH)) return true;
		struct Enemy *ttemp = temp;
		temp = temp->next;
		if(kill_flag) delete_enemy(ttemp);
	}
	return false;
}


struct Bullet
{
	int x,y;
	int w,h;
	struct Bullet *next;
} *bullet_head, *bullet_avail, bullet[NR_BULLET];

bool generate_bullet()
{
	if(!enable_bullet) return false;
	if(bullet_avail)
	{
		struct Bullet *temp = bullet_avail;
		bullet_avail = bullet_avail->next;
		temp->x = x_player + LS_WIDTH/2; temp->y = SCR_HEIGHT - LS_HEIGHT;
		temp->w = BT_WIDTH; temp->h = BT_HEIGHT;
		temp->next = bullet_head;
		bullet_head = temp;
		return true;
	}
	else return false;
}

void delete_bullet(struct Bullet *bullet_p)
{
	struct Bullet *temp = bullet_head;
	if(temp == bullet_p)
	{
		bullet_head = bullet_p->next;
		bullet_p->next = bullet_avail;
		bullet_avail = bullet_p;
	}
	else
	{
		while(temp->next != bullet_p) temp = temp->next;
		temp->next = bullet_p->next;
		bullet_p->next = bullet_avail;
		bullet_avail = bullet_p;
	}
}

bool update_bullet()
{
	struct Bullet *temp = bullet_head;
	bool kill_flag;
	bool return_flag = false;
	while(temp)
	{
		kill_flag = false;
		temp->y --;
		if(temp->y < 0) kill_flag = true;
		else
		{
			struct Enemy *enemy_temp = enemy_head;
			while(enemy_temp)
			{
				if(enemy_temp->y >= temp->y - FF_HEIGHT && (enemy_temp->x + FF_WIDTH >= temp->x && enemy_temp->x <= temp->x + BT_WIDTH))
				{
					delete_enemy(enemy_temp);
					kill_flag = true;
					break;
				}
				enemy_temp = enemy_temp->next;
			}
		}

		struct Bullet *ttemp = temp;
		temp = temp->next;
		if(kill_flag)
		{
			delete_bullet(ttemp);
			return_flag = true;
		}
	}
	return return_flag;
}

void update_player()
{
	if(x_player + inc_player >= 0 && x_player + inc_player < SCR_WIDTH - LS_WIDTH)
		x_player += inc_player;
}


void init_effect()
{
	int i;
	inc_player = 0;
	x_player = 0;
	enable_bullet = false;
	enemy_head = NULL;
	enemy_avail = enemy;
	for(i = 0; i < NR_ENEMY - 1; ++ i)
	{
		enemy[i].next = enemy + i + 1;
	}
	enemy[NR_ENEMY - 1].next = NULL;
	bullet_head = NULL;
	bullet_avail = bullet;
	for(i = 0; i < NR_BULLET - 1; ++ i)
	{
		bullet[i].next = bullet + i + 1;
	}
	bullet[NR_BULLET - 1].next = NULL;
}


void draw_screen()
{
	clear_buffer();

	drawRect_buffer(gImage_chino, x_player, SCR_HEIGHT - LS_HEIGHT, LS_WIDTH, LS_HEIGHT);
	struct Enemy *enemy_temp = enemy_head;
	while(enemy_temp)
	{
		drawRect_buffer(gImage_tibi, enemy_temp->x, enemy_temp->y, FF_WIDTH, FF_HEIGHT);
		enemy_temp = enemy_temp->next;
	}
	struct Bullet *bullet_temp = bullet_head;
	while(bullet_temp)
	{
		drawRect_buffer(gImage_Bullet, bullet_temp->x, bullet_temp->y, BT_WIDTH, BT_HEIGHT);
		bullet_temp = bullet_temp->next;
	}

	display_buffer();
}
