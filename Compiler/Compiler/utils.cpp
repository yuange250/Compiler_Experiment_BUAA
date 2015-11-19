#include "stdafx.h"
#include "globals.h"
void getsym();
bool ifin(string symbol, string symbols[])
{
	int i = 0;
	while (symbols[i]!="")
	{
		if (symbol == symbols[i])
			return true;
		i++;
	}
	return false;
}

void error(int error_no)
{
	//print out 
	if (error_no == 1)
		printf("illegal form in char!correct form:'1' or 'a'\n");
	else if (error_no == 2)
		printf("illegal end of the file\n");
	else if (error_no == 3)
		printf("illegal charactor\n");
	else if (error_no == 4)
		printf("illegal charactor in string!\n");
	else if (error_no == 5)
		printf("no such symbol");
	else if (error_no == 6)
		printf("should be an ident+");
	else if (error_no == 7)
		printf("type error!");
	else if (error_no == 8)
		printf(") absence");
	else if (error_no == 9)
		printf("too little parameters");
	else if (error_no == 10)
		printf("[ absence");
	else if (error_no == 11)
		printf("] absence");
	else if (error_no == 12)
		printf("redefine error");
	else if (error_no == 13)
		printf("do lost");
	err++;
	printf("error occur!\n");
}

void test(string s1[], string s2[], int error_no)
{
	if (!ifin(sym, s1))
	{
		error(error_no);
		while (!(ifin(sym, s1) || ifin(sym, s2)))
			getsym();
	}
}

int position(string id){
	int i = 0;
	id_table[0].name = id;
	i = tx;
	while (id_table[i].name != id&&i!=0)
	{
		i = i - 1;
	}
	return i;
}
int check_ifexist(string id)
{
	int i = 0;
	id_table[0].name = id;
	i = tx;
	while (id_table[i].name != id&&i != 0)
	{
		i = i - 1;
		if (id_table[i].lev != level)
		{
			return 0;
		}
	}
	return i;
}