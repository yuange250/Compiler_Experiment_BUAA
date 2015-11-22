#include "stdafx.h"
#include "globals.h"
void error(int error_no);
void test(string *s1, string *s2, int error_no);
bool ifin(string symbol, string *symbols);
void getsym();
void block();
int check_ifexist(string id);
void generate(string opr, string src1, string src2, string des);
void enter(string type)//废弃的矿坑
{
	tx++;
	id_table[tx].name = iden;
	id_table[tx].obj = type;


}
void enterconst()//向符号表中登陆常数，很简单，同时将值存进去
{

	tx++;
	id_table[tx].name = iden;
	if (sym == "uinteger")
	{
		id_table[tx].type = "integersym";
		id_table[tx].value = number;
	}
	else if (sym == "char")
	{
		id_table[tx].type = "charsym";
		id_table[tx].value = iden[0];
	}
	id_table[tx].adr = sp_addr-sp_piece_top;//存相对于栈帧顶的位移

	id_table[tx].lev = level;
	id_table[tx].obj = "const";
	id_table[tx].able = true;
	id_table[tx].param_list = NULL;
	id_table[tx].arrayinfo = NULL;
}
void const_declaration()//这与下面所有declaration类似,都是递归下降的一部分，程序分析的时候，碰到const就进入这个函数
{
	printf("now in constdeclaration\n");
	bool minus_symbol = false;
	do
	{
		if (sym == "ident")
		{
			getsym();
			if (sym == "eql" || sym == "becomes")
			{
				if (sym == "becomes")
					error(13);
				getsym();
				if (sym == "plus" || sym == "minus")
				{
					getsym();
					if (sym == "minus")
						minus_symbol=true;
				}
				if (sym == "uinteger" || sym == "char")
				{
					if (minus_symbol)
						number = -number;
					if (check_ifexist(iden) != 0)
					{
						error(1);
					}



					enterconst();
					getsym();
				}
				else
					error(31);
			}
			else
				error(32);
		}
		else
			error(2);
		
	} while (sym == ",");
	if (sym != "semicolon")
		error(33);//缺少分号
	else
		getsym();
	string correctsymbols[] = {"varsym","funcsym","procsym","beginsym",""};
	string continuesymbols[] = {""};
	test(correctsymbols,continuesymbols,40);
}
void entervar(bool if_params)//将变量的基本信息登录到符号表中，因为pascal文法的限制，var x,y,z：char;所以需要现将名字登陆进符号表，记录开始位置，然后去填类型
{
	tx++;
	id_table[tx].name = iden;
	if (if_params)
		id_table[tx].lev = level + 1;
	else
		id_table[tx].lev = level;
	id_table[tx].obj = "var";
	id_table[tx].able = true;
	id_table[tx].param_list = NULL;
	id_table[tx].arrayinfo = NULL;
}
void entervartype(string type, int tx_start){//这个是填入变量基本类型的函数,tx_start已经在variable_declaration中记录好,从这个节点开始填入变量类型。
	int index = tx_start + 1;
	while (index <= tx)
	{
		id_table[index].type = type;
		index++;
	}
}
void enterarray(string type,int size,int tx_start)//变量类型是数组的情况。稍微有点复杂，因为我在table节点中单独加了一个指向array_info的指针，array_info中存储了array的相关信息。
{
	int index = tx_start + 1;
	while (index <= tx)
	{
		id_table[index].type = "array";
		id_table[index].arrayinfo = (struct array_info *)calloc(1,sizeof(ARRAY_INFO));
		id_table[index].arrayinfo->size = size;
		id_table[index].arrayinfo->type = type;
		index++;
	}
}
void variable_declaration()
{
	printf("now in variableblock\n");
	do{
		int curren_index = tx;//先记住当前符号表的位置
		if (sym == "ident")
		{
			if (check_ifexist(iden) != 0)
			{
				error(1);
			}


			entervar(false);
			getsym();
		}
		else
			error(2);
		while (sym == "comma")
		{
			getsym();
			if (sym == "ident")
			{
				if (check_ifexist(iden) != 0)
				{
					error(1);
				}


				entervar(false);
				getsym();

			}
			else
				error(2);
		}
		if (sym == "colon")
			getsym();
		else
			error(34);
		if (sym == "integersym" || sym == "charsym")
		{
			entervartype(sym, curren_index);
			getsym();//此处应该填表的，将类型填进去。
		}
		else if (sym == "arraysym")
		{
			int size = 0;
			getsym();
			if (sym == "lbracket")
				getsym();
			else
				error(10);//缺少左中括号
			if (sym != "uinteger")
				error(35);//数组下标错误
			else
			{
				size = number;
				getsym();
			}
			if (sym != "rbracket")
				error(11);//缺少右中括号。
			else
				getsym();
			if (sym != "ofsym")
				error(7);//缺少of
			else
				getsym();
			if (sym == "charsym" || sym == "integersym")
			{
				enterarray(sym, size, curren_index);
				getsym();
			}
			else
				error(12);//应该为基本类型
		}
		else
			error(18);
		if (sym != "semicolon")
			error(33);
		else
			getsym();
	} while (sym == "ident");
	string correctsymbols[] = { "funcsym", "procsym", "beginsym", "" };
	string continuesymbols[] = { "" };
	test(correctsymbols, continuesymbols, 40);
}
void enterfunction()//填入function的基本信息
{
	tx++;
	id_table[tx].name = iden;
	id_table[tx].lev = level;
	id_table[tx].obj = "function";
	id_table[tx].able = true;
	id_table[tx].param_list = NULL;
	id_table[tx].arrayinfo = NULL;
}
void func_parameters()//函数参数，填入进param_list指针所指的空间，
{
	printf("now in func_parameter\n");
	id_table[tx].param_list = (struct params *)calloc(1, sizeof(PARAMS));
	id_table[tx].param_list->param_num=0;
	int current_tx = tx;
	int bottom_index = 0;
	int top_index = 0;

	do{
		bottom_index = top_index;
		if (sym == "semicolon")
			getsym();
		if (sym == "varsym")
			getsym();
		if (sym == "ident")
		{
			if (check_ifexist(iden) != 0)
			{
				error(1);
			}
			id_table[current_tx].param_list->param_names[top_index++] = sym;
			id_table[current_tx].param_list->param_num++;
			entervar(true);
			getsym();
		}
		else
			error(2);
		while (sym == "comma"&&id_table[current_tx].param_list->param_num<param_max)
		{
			getsym();
			if (sym == "ident")
			{
				if (check_ifexist(iden) != 0)
				{
					error(1);
				}
				id_table[current_tx].param_list->param_names[top_index++] = sym;//参数的名字
				id_table[current_tx].param_list->param_num++;
				entervar(true);
				getsym();
			}
		}
		if (sym == "colon")
			getsym();
		else
			error(34);
		if (sym == "integersym" || sym == "charsym")
		{
			for (int i = bottom_index; i < top_index; i++)
			{
				id_table[current_tx].param_list->types[i] = sym;//参数的类型。
				id_table[current_tx + 1 + i].type=sym;
			}
			getsym();//此处应该填表的，将类型填进去。
		}
		else
			error(12);
	} while (sym == "semicolon");
}

void function_declaration()
{//函数声明部分
	printf("now in funcdeclaration\n");
	
	if (sym == "ident")
	{
		if (check_ifexist(iden) != 0)
		{
			error(1);
		}
		generate("LABEL","","",iden);

		enterfunction();
		getsym();
	}
	else
		error(2);//应该为标识符。
	int current_tx = tx;
	if (sym == "lparen")
	{
		getsym();
		func_parameters();
		if (sym == "rparen")
			getsym();
		else
			error(3);//缺失右括号。
	}
	if (sym != "colon")
		error(34);//缺失冒号
	else
		getsym();
	if (sym == "charsym" || sym == "integersym")
	{//如果还有一点，那就是一定要把函数的返回值填进去
		id_table[current_tx].type = sym;
		getsym();
	}
	else
		error(12);//基本类型缺失
	if (sym != "semicolon")
		error(99);//缺少分号。
	else
		getsym();
	block();
	if (sym != "semicolon")
		error(33);//缺失分号
	else
		getsym();
	string correctsymbols[] = {  "funcsym", "procsym", "beginsym", "" };
	string continuesymbols[] = { "" };
	test(correctsymbols, continuesymbols, 40);
}
void enterprocedure()
{
	tx++;
	id_table[tx].name = iden;
	id_table[tx].lev = level;
	id_table[tx].obj = "procedure";
	id_table[tx].type = "null";
	id_table[tx].able = true;
	id_table[tx].param_list = NULL;
	id_table[tx].arrayinfo = NULL;
}
void procedure_declaration()
{//函数与过程的区别在于没有返回值而已
	printf("now in proceduredeclaration\n");
	if (sym == "ident")
	{
		if (check_ifexist(iden) != 0)
		{
			error(1);
		}
		generate("LABEL", "", "", iden);

		enterprocedure();
		getsym();
	}
	else
		error(2);//应该为标识符。
	if (sym == "lparen")
	{
		getsym();
		func_parameters();
		if (sym == "rparen")
			getsym();
		else
			error(3);//缺失右括号。
	}
	if (sym != "semicolon")
		error(33);//缺少分号。
	else
		getsym();
	block();
	if (sym != "semicolon")
		error(33);//缺失分号
	else
		getsym();
	string correctsymbols[] = { "funcsym", "procsym", "beginsym", "" };
	string continuesymbols[] = { "" };
	test(correctsymbols, continuesymbols, 40);
}