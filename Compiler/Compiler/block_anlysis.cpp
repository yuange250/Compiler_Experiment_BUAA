#include "stdafx.h"
#include "globals.h"
void getsym();
void const_declaration();
void variable_declaration();
void function_declaration();
void procedure_declaration();
void multi_statement();
void block()
{
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
}