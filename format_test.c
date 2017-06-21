#include <string.h>
#include <stdio.h>

int main()
{
	FILE *fp1;
	FILE *fp2;
	char poem[10][30];
	int i;
	memset(poem,0,sizeof(poem));
	strcpy(poem[0],"Gou li guo jia");
	strcpy(poem[1],"sheng si yi,");
	strcpy(poem[2],"Qi yin huo fu");
	strcpy(poem[3],"bi qu zhi");
	fp1 = fopen("readpoem.txt","wb");
	for (i = 0;i < 4;i++)
	{
		fwrite(poem[i],1,30,fp1);
	}
	fclose(fp1);
	return 0;
}
