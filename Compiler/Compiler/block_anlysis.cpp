#include "stdafx.h"
#include "globals.h"
void error(int error_no);
void getsym();
void const_declaration();
void variable_declaration();
void function_declaration();
void procedure_declaration();
void multi_statement();
void test(string *s1, string *s2, int error_no);
int ifin(string symbol, string * symbols);
void generate(string opr, string src1, string src2, string des);
string generate_func_proc_label(string name, int code);
void clear(int level)
{
	int i = tx;
	while (id_table[i].lev >= level)
	{
		/*
		if (id_table[i].arrayinfo != NULL)
		{
			free(id_table[i].arrayinfo);
			id_table[i].arrayinfo = NULL;
		}
		if (id_table[i].param_list != NULL)
		{
			free(id_table[i].param_list);
			id_table[i].param_list = NULL;
		}
		*/
		id_table[i].able = false;
		i--;
	}
}
void block(string func_name,int code)
{
	temp_var_num = 0;
	level++;
//	printf("now in block\n");
//	do{
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
		string correctsymbols[] = { "beginsym", "" };
		
		test(correctsymbols, declbegsys, 30);
//	} while (ifin(sym,declbegsys));
	if (sym == "beginsym")
	{
		generate("PLABEL", "", "", generate_func_proc_label(func_name, code));
		multi_statement();
	}
	else
	{
		error(30);
		//µ±È»¾Í´íÀ²£¡
	}
	clear(level);
	level--;
}
