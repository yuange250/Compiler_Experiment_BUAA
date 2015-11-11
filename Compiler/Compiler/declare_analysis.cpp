#include "stdafx.h"
#include "globals.h"
void error(int error_no);
void test(string s1[], string s2[], int error_no);
bool ifin(string symbol, string symbols[]);
void getsym();
void block();
void enter(string type)
{
	tx++;
	id_table[tx].name = iden;
	id_table[tx].kind = type;

}
void const_declaration()
{
	printf("now in constdeclaration\n");
	do
	{
		if (sym == "ident")
		{
			getsym();
			if (sym == "eql" || sym == "becomes")
			{
				if (sym == "becomes")
					error(1);
				getsym();
				if (sym == "plus" || sym == "minus")
				{
					getsym();
					if (sym == "minus")
						number = -number;
				}
				if (sym == "uinteger" || sym == "char")
				{
					enter("constant");
					getsym();
				}
				else
					error(2);
			}
			else
				error(3);
		}
		else
			error(4);
	} while (sym == ",");
	if (sym != "semicolon")
		error(99);//缺少分号
	else
		getsym();
}
void variable_declaration()
{
	printf("now in variableblock\n");
	do{
		if (sym == "ident")
		{
			getsym();
		}
		else
			error(99);
		while (sym == "comma")
		{
			getsym();
			if (sym == "ident")
			{
				getsym();
			}
		}
		if (sym == "colon")
			getsym();
		else
			error(99);
		if (sym == "integer" || sym == "charsym")
			getsym();//此处应该填表的，将类型填进去。
		else if (sym == "arraysym")
		{
			getsym();
			if (sym == "lbracket")
				getsym();
			else
				error(99);//缺少左中括号
			if (sym != "uinteger")
				error(99);//数组下标错误
			else
				getsym();
			if (sym != "rbracket")
				error(99);//缺少右中括号。
			else
				getsym();
			if (sym != "of")
				error(99);//缺少of
			else
				getsym();
			if (sym == "charsym" || sym == "uinteger")
			{
				getsym();
			}
			else
				error(99);//应该为基本类型
		}
		if (sym != "semicolon")
			error(99);
		else
			getsym();
	} while (sym == "ident");
}
void func_parameters()
{
	printf("now in func_parameter\n");
	do{
		if (sym == "semicolon")
			getsym();
		if (sym == "varsym")
			getsym();
		if (sym == "ident")
		{
			getsym();
		}
		else
			error(99);
		while (sym == "comma")
		{
			getsym();
			if (sym == "ident")
			{
				getsym();
			}
		}
		if (sym == "colon")
			getsym();
		else
			error(99);
		if (sym == "integer" || sym == "charsym")
			getsym();//此处应该填表的，将类型填进去。
		else
			error(99);
	} while (sym == "semicolon");
}
void function_declaration()
{
	printf("now in funcdeclaration\n");
	if (sym != "ident")
		getsym();
	else
		error(99);//应该为标识符。
	if (sym == "lparen")
	{
		getsym();
		func_parameters();
		if (sym == "rparen")
			getsym();
		else
			error(99);//缺失右括号。
	}
	if (sym != "colon")
		error(99);//缺失冒号
	else
		getsym();
	if (sym == "charsym" || sym == "intsym")
	{
		getsym();
	}
	else
		error(99);//基本类型缺失
	if (sym != "semicolon")
		error(99);//缺少分号。
	else
		getsym();
	block();
	if (sym != "semicolon")
		error(99);//缺失分号
	else
		getsym();
}
void procedure_declaration()
{//函数与过程的区别在于
	printf("now in proceduredeclaration\n");
	if (sym == "ident")
		getsym();
	else
		error(99);//应该为标识符。
	if (sym == "lparen")
	{
		getsym();
		func_parameters();
		if (sym == "rparen")
			getsym();
		else
			error(99);//缺失右括号。
	}
	if (sym != "semicolon")
		error(99);//缺少分号。
	else
		getsym();
	block();
	if (sym != "semicolon")
		error(99);//缺失分号
	else
		getsym();
}