#include "stdafx.h"
#include "globals.h"
void getsym();
bool ifin(string symbol, string symbols[])
{
	int i = 0;
	while (i < sizeof(symbols) / sizeof(string))
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

int position(string id,int tx){
	int i = 0;
	id_table[0].name = id;
	i = tx;
	while (id_table[i].name != id&&i!=0)
	{
		i = i - 1;
	}
	return i;
}