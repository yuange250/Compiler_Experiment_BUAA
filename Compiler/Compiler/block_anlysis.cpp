#include "stdafx.h"
#include "globals.h"
void getsym();
void const_declaration();
void variable_declaration();
void function_declaration();
void procedure_declaration();
void multi_statement();
void clear(int level)
{
	while (id_table[tx].lev == level)
	{
		if (id_table[tx].arrayinfo != NULL)
		{
			free(id_table[tx].arrayinfo);
			id_table[tx].arrayinfo = NULL;
		}
		if (id_table[tx].param_list != NULL)
		{
			free(id_table[tx].param_list);
			id_table[tx].param_list = NULL;
		}
		id_table[tx].able = false;
		tx--;
	}
}
void block()
{
	level++;
	printf("now in block\n");
	if (sym == "constsym")
	{
		getsym();
		const_declaration();
	}
	if (sym == "varsym")
	{
		getsym();
		variable_declaration();
	}
	while (sym == "procsym" || sym == "funcsym")
	{
		if (sym == "procsym")
		{
			getsym();
			procedure_declaration();
		}
		else
		{
			getsym();
			function_declaration();
		}
	}
	if (sym == "beginsym")
	{
		multi_statement();
	}
	else
	{
		//µ±È»¾Í´íÀ²£¡
	}
	clear(level);
	level--;
}
