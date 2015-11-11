#include "stdafx.h"
#include "globals.h"
void error(int error_no);
void test(string s1[], string s2[], int error_no);
bool ifin(string symbol, string symbols[]);
void getsym();
void expression();
void multi_statement();
void function_symbols(){
	
	if (sym == "(")
	{
		int paramnum;//参数个数。
	}
}
void factor(){
	printf("now in factor\n");
	int i = 0;
//	test(facbegsys,fsys,24);
	while (ifin(sym,facbegsys))
	{
		if (sym == "ident")
		{
//			int i = position();词法部分。
			//此时需要在表中查找，是否是函数。
			//if是函数。。。。。。进入函数参数列表部分。
			getsym();
			
		}
		else if (sym == "uinteger")
		{
			int num = number;
			if (num > amax)
			{
				error(11);
			}
			getsym();
		}
		else if (sym == "lparen")
		{
			getsym();
/*			int length_temp = sizeof(fsys) / sizeof(string);	
			string fsys_temp[sizeof(fsys)/sizeof(string)+1];
			memcpy(fsys_temp, fsys, length_temp);
			fsys_temp[length_temp] = "rparen";
*/			expression();
			if (sym == "rparen")
			{
				getsym();
			}
			else
			{
				error(22);
			}
		}
	}
}
void term()
{
	printf("now in term\n");
	factor();
	while (sym == "times" || sym == "slash")
	{
		getsym();
		factor();
	}
}
void expression()
{
	if (sym == "ident")
	{
		//	getsym();
	}
	else
		error(99);
	printf("now in expression\n");
	if (sym=="plus"||sym=="minus")
	{
		getsym();
	}
	term();
	while (sym == "plus" || sym == "minus")
	{
		getsym();
		term();
	}
}
void condition()
{
	printf("now in condition\n");
	expression();
	getsym();
	string compare_symbols[] = { "eql", "neq", "lss", "leq", "gtr", "geq" };
	if (ifin(sym,compare_symbols))
	{
		getsym();
		expression();
	}
	else
		error(20);
}
void statement()
{
	printf("now in statement\n");
	if (sym == "ident")
	{
		//判断是不是函数。若是的话，也没什么，卧槽函数部分好傻逼啊，难道是弱类型的语言吗，可以随便赋值。
		printf("now in assign_statement\n");
		getsym();
		if (sym == "becomes")
			getsym();
		else
			error(13);
		expression();
	}
	else if (sym == "callsym")//这儿文法是有问题的呀！！！！！！！！调用过程不需要call的呀！！！！
	{
		printf("now in call_statement\n");
		getsym();
		if (sym == "ident")
			getsym();
		else
			error(99);//call
	}
	else if (sym == "ifsym")
	{
		printf("now in if_statement\n");
		getsym();
		condition();
		if (sym == "thensym")
			getsym();
		else
			error(16);
		statement();
		if (sym == "else")
			statement();
	}
	else if (sym == "whilesym")
	{
		printf("now in whilestatement\n");
		getsym();
		condition();
		if (sym == "dosym")
			getsym();
		else
			error(18);
		statement();
	}
	else if (sym == "beginsym")
	{
		printf("now in begin_statement\n");
		multi_statement();
	}
	else if (sym == "readsym")
	{
		printf("now in readstatement\n");
		getsym();
		if (sym == "lparen")
		{
			do{
				getsym();
				if (sym == "ident")
				{

				}
				else
					error(4);
				getsym();
			} while (sym == "comma");
		}
		else
			error(40);
		if (sym != "rparen")
			error(22);
		getsym();
	}
	else if (sym == "writesym")
	{
		printf("now in writestatement\n");
		getsym();
		if (sym == "lparen")
		{
			do{
				getsym();
				expression();
			} while (sym == "comma");
			if (sym != "rparen")
				error(22);
			getsym();
		}
		else
			error(40);
	}
	else if (sym == "forsym")
	{
		printf("now in forstatement\n");
		getsym();
		if (sym == "ident")
		{
			getsym();	
		}
		else
			error(4);
		if (sym == "becomes")
		{
			getsym();
			expression();
		}
		else
			error(40);
		if (sym == "downto" || sym == "to")
		{
			getsym();
			expression();
		}
		else
			error(40);
	}
}
void multi_statement()
{
	getsym();
	statement();
	while (sym == "semicolon")
	{
		getsym();
		statement();
	}
	if (sym == "endsym")
		getsym();
	else
		error(17);
} 