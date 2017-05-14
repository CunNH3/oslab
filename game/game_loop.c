#include "include/types.h"
//#include "include/assert.h"
#include "include/keyboard.h"
#include "include/video.h"

typedef struct
{
	int Number;
	int item;//0,1,2
	int Number_aph;
}SQUARE;

typedef struct 
{
	bool ifend;
	int item;
}END;

SQUARE squares[3][3];

bool judge(int a,int b,int c,int d)
{
	if (squares[a][b].item == squares[c][d].item) return 1; else return 0;
}


void init_squares()
{
	int i,j;
	for (i = 0;i < 3;i++)
	{
		for (j = 0;j < 3;j++)
		{
			squares[i][j].item = 0;
			squares[i][j].Number = i * 3 + j + 1;
			if (squares[i][j].Number == 1) squares[i][j].Number_aph = 16;
			else 
			if (squares[i][j].Number == 2) squares[i][j].Number_aph = 22;
			else 
			if (squares[i][j].Number == 3) squares[i][j].Number_aph = 4;
			else 
			if (squares[i][j].Number == 4) squares[i][j].Number_aph = 0;
			else 
			if (squares[i][j].Number == 5) squares[i][j].Number_aph = 18;
			else 
			if (squares[i][j].Number == 6) squares[i][j].Number_aph = 3;
			else 
			if (squares[i][j].Number == 7) squares[i][j].Number_aph = 25;
			else 
			if (squares[i][j].Number == 8) squares[i][j].Number_aph = 23;
			else 
			if (squares[i][j].Number == 9) squares[i][j].Number_aph = 2;
			else squares[i][j].Number_aph = 30;
		}
	}	
}


END judge_end()
{
	int i,j;
	bool flag = true;
	END temp;
	temp.ifend = false;
	if ((squares[0][0].item != 0) && (judge(0,0,0,1)) && (judge(0,1,0,2)))
	{
		temp.ifend = true;
		temp.item = squares[0][0].item;
		flag = false;
	}
	else
	if ((squares[0][0].item != 0) && (judge(0,0,1,0)) && (judge(0,0,2,0)))
	{
		temp.ifend = true;
		temp.item = squares[0][0].item;
		flag = false;
	}
	else
	if ((squares[2][0].item != 0) && (judge(2,0,2,1)) && (judge(2,0,2,2)))
	{
		temp.ifend = true;
		temp.item = squares[2][0].item;
		flag = false;
	}
	else 
	if ((squares[0][2].item != 0) && (judge(0,2,1,2)) && (judge(0,2,2,2)))
	{
		temp.ifend = true;
		temp.item = squares[0][2].item;
		flag = false;
	}
	else 
	if ((squares[1][0].item != 0) && (judge(1,0,1,1)) && (judge(1,0,1,2)))
	{
		temp.ifend = true;
		temp.item = squares[1][0].item;
		flag = false;
	}
	else 
	if ((squares[0][1].item != 0) && (judge(0,1,1,1)) && (judge(0,1,2,1)))
	{
		temp.ifend = true;
		temp.item = squares[0][1].item;
		flag = false;
	}
	else 
	if ((squares[0][0].item != 0) && (judge(0,0,1,1)) && (judge(0,0,2,2)))
	{
		temp.ifend = true;
		temp.item = squares[0][0].item;
		flag = false;
	}
	else 
	if ((squares[0][2].item != 0) && (judge(0,2,1,1)) && (judge(0,2,2,0)))
	{
		temp.ifend = true;
		temp.item = squares[0][2].item;
		flag = false;
	}
	else 
	{
		temp.ifend = false;
		temp.item = 0;
		for (i = 0;i < 3;i++)
			for (j = 0;j < 3;j++)
				if (squares[i][j].item == 0) flag = false;
	}
	if (flag == true)
	{
		temp.ifend = true;
		temp.item = 3;
	}
	return temp;

}

void game_loop()
{
	int i,j,k; 
	int count = 0;
	while (1)
	{
		for (i = 0;i < 26;i++)
		{
			if (query_key(i))
			{;
				for (j = 0;j < 3;j++)
				{
					for (k = 0;k < 3;k++)
					{
						if ((squares[j][k].Number_aph == i) && (squares[j][k].item == 0))
						{
							if (count % 2 == 0)
							{
								draw_squares(squares[j][k].Number,1);
								squares[j][k].item = 1;
								display();
							}
							else
							{
								draw_squares(squares[j][k].Number,13);
								squares[j][k].item = 2;
								display();
							}
							count++;
							release_key(i);
						}
					}
				}	
			}
		}
		END judge = judge_end();
		if ((judge.ifend == true) && ((judge.item - 1) == 0))
		{
			draw_white_screen();
			break;
		}
		else 
		if ((judge.ifend == true) && ((judge.item - 2) == 0))
		{
			draw_blue_screen();
			break;
		}
		else 
		if ((judge.ifend == true) && (judge.item - 3) == 0)
		{
			draw_pink_screen();
			break;
		}
	}
}
