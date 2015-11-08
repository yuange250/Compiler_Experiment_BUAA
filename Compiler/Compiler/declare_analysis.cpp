#include "stdafx.h"
#include "globals.h"
void error(int error_no);
void test(string s1[], string s2[], int error_no);
bool ifin(string symbol, string symbols[]);
void getsym();
void enter(string type)
{
	tx++;
	id_table[tx].name = iden;
	id_table[tx].kind = type;

}
void const_declaration()
{
	if (sym == "ident")
	{
		getsym();
		if (sym == "eql"||sym=="becomes")
		{
		    if (sym == "becomes")
			error(1);
			getsym();
			if (sym == "number")
			{
				enter("constant");
				getsym();
			}
		}
		else
			error(3);
	}
	else
		error(4);
}
void variable_declaration()
{

}
void function_declaration()
{
}