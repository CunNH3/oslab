#include "include/types.h"
//#include "include/assert.h"
#include "include/keyboard.h"
#include "include/video.h"

typedef struct
{
	int num;
	int isempty;//0,1,2
	int num_aph;
}SQUARE;

typedef struct 
{
	bool ifend;
	int isempty;
}END;

SQUARE squares[3][3];

void init_squares()
{
	int i,j;
	for (i = 0;i < 3;i++)
	{
		for (j = 0;j < 3;j++)
		{
			squares[i][j].isempty = 0;
			squares[i][j].num = i * 3 + j + 1;
			if (squares[i][j].num == 1) squares[i][j].num_aph = 16;
			else 
			if (squares[i][j].num == 2) squares[i][j].num_aph = 22;
			else 
			if (squares[i][j].num == 3) squares[i][j].num_aph = 4;
			else 
			if (squares[i][j].num == 4) squares[i][j].num_aph = 0;
			else 
			if (squares[i][j].num == 5) squares[i][j].num_aph = 18;
			else 
			if (squares[i][j].num == 6) squares[i][j].num_aph = 3;
			else 
			if (squares[i][j].num == 7) squares[i][j].num_aph = 25;
			else 
			if (squares[i][j].num == 8) squares[i][j].num_aph = 23;
			else 
			if (squares[i][j].num == 9) squares[i][j].num_aph = 2;
			else squares[i][j].num_aph = 30;
		}
	}	
}

END judge_end()
{
	int i,j;
	bool flag = true;
	END temp;
	temp.ifend = false;
	if ((squares[0][0].isempty != 0) && (squares[0][0].isempty == squares[0][1].isempty) && (squares[0][1].isempty == squares[0][2].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[0][0].isempty;
		flag = false;
	}
	else
	if ((squares[0][0].isempty != 0) && (squares[0][0].isempty == squares[1][0].isempty) && (squares[0][0].isempty == squares[2][0].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[0][0].isempty;
		flag = false;
	}
	else
	if ((squares[2][0].isempty != 0) && (squares[2][0].isempty == squares[2][1].isempty) && (squares[2][0].isempty == squares[2][2].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[2][0].isempty;
		flag = false;
	}
	else 
	if ((squares[0][2].isempty != 0) && (squares[0][2].isempty == squares[1][2].isempty) && (squares[0][2].isempty == squares[2][2].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[0][2].isempty;
		flag = false;
	}
	else 
	if ((squares[1][0].isempty != 0) && (squares[1][0].isempty == squares[1][1].isempty) && (squares[1][0].isempty == squares[1][2].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[1][0].isempty;
		flag = false;
	}
	else 
	if ((squares[0][1].isempty != 0) && (squares[0][1].isempty == squares[1][1].isempty) && (squares[0][1].isempty == squares[2][1].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[0][1].isempty;
		flag = false;
	}
	else 
	if ((squares[0][0].isempty != 0) && (squares[0][0].isempty == squares[1][1].isempty) && (squares[0][0].isempty == squares[2][2].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[0][0].isempty;
		flag = false;
	}
	else 
	if ((squares[0][2].isempty != 0) && (squares[0][2].isempty == squares[1][1].isempty) && (squares[0][2].isempty == squares[2][0].isempty))
	{
		temp.ifend = true;
		temp.isempty = squares[0][2].isempty;
		flag = false;
	}
	else 
	{
		temp.ifend = false;
		temp.isempty = 0;
		for (i = 0;i < 3;i++)
			for (j = 0;j < 3;j++)
				if (squares[i][j].isempty == 0) flag = false;
	}
	if (flag == true)
	{
		temp.ifend = true;
		temp.isempty = 3;
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
						if ((squares[j][k].num_aph == i) && (squares[j][k].isempty == 0))
						{
							if (count % 2 == 0)
							{
								draw_squares(squares[j][k].num,15);
								squares[j][k].isempty = 1;
								display();
							}
							else
							{
								draw_squares(squares[j][k].num,1);
								squares[j][k].isempty = 2;
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
		if ((judge.ifend == true) && ((judge.isempty - 1) == 0))
		{
			white_screen();
			break;
		}
		else 
		if ((judge.ifend == true) && ((judge.isempty - 2) == 0))
		{
			blue_screen();
			break;
		}
		else 
		if ((judge.ifend == true) && (judge.isempty - 3) == 0)
		{
			yellow_screen();
			break;
		}
	}
}
