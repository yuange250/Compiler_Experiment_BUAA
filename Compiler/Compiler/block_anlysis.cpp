#include "stdafx.h"
#include "globals.h"
void error(int error_no);
void getsym();
void const_declaration();
void variable_declaration();
void function_declaration();
void procedure_declaration();
void multi_statement();
<<<<<<< HEAD
void test(string *s1, string *s2, int error_no);
int ifin(string symbol, string * symbols);
=======
void generate(string opr, string src1, string src2, string des);
>>>>>>> origin/master
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
<<<<<<< HEAD
	temp_var_num = 0;
=======
	int codes_index_temp = code_index;
	int sp_addr_temp = sp_addr_temp;
	sp_piece_top
	generate("SUB", "sp_addr", "", "sp_addr");
>>>>>>> origin/master
	level++;
	printf("now in block\n");
	do{
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
	} while (ifin(sym,declbegsys));
	if (sym == "beginsym")
	{
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
