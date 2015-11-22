//line348 打印string需要继续细想，可能需要放到.data中！
#include "stdafx.h"
#include "globals.h"
void error(int error_no);
int position(string id);
void test(string *s1, string *s2, int error_no);
int ifin(string symbol, string *symbols);
void getsym();
void expression(string &result);
void multi_statement();
string generate_temp_var();
void generate(string opr, string src1, string src2, string des);
string generate_label();
void function_params(int posi){//这个也是废弃的矿坑,好吧，这不是
	string result;
	if (sym == "lparen")
	{
		int nums = 0;
		do{
			getsym();
			expression(result);
			nums++;
		} while (sym == "comma");

		if (sym != "rparen")
		{
			error(3);
		}
		if (nums != id_table[posi].param_list->param_num)
		{
			error(21);
		}
		//int paramnum;//参数个数。
	}

}
void array_ident(string &result)
{

	if (sym != "lbracket")
	{
		error(10);
	}
	getsym();
	expression(result);
	if (sym != "rbracket")
	{
		error(11);
	}
}
void factor(string &result){
	printf("now in factor\n");
	int i = 0;
	//	test(facbegsys,fsys,24);
	//string correctsymbols[] = { "ident", "ifsym", "dosym", "beginsym", "readsym", "writesym", "forsym", "" };
	string continuesymbols[] = { "semicolon", "endsym", "times", "plus", "slash", "minus", "eql", "neq", "lss", "leq", "gtr", "geq", "thensym","tosym","downtosym","rparen","rbracket","dosym","whilesym","comma","" };
	test(facbegsys, continuesymbols, 36);
	while (ifin(sym, facbegsys))
	{
		if (sym == "ident")
		{
			int posi = position(iden);
			//			int i = position();词法部分。
			//此时需要在表中查找，是否是函数。
			//if是函数。。。。。。进入函数参数列表部分。
			if (posi == 0)
			{
				error(0);
			}
			else
			{
				if (id_table[posi].obj == "function")
				{
					getsym();
					function_params(posi);
				}
				else if (id_table[posi].type == "array")
				{
					string src1;
					string src2;
					string des;
					src1 = id_table[posi].name;
					getsym();//是否对数组越界进行检查，，，，
					array_ident(src2);
					des = generate_temp_var();
					generate("LOAD",src1,src2,des);
					result = des;
				}
				else
				{
					result = iden;
				}
			}
			getsym();
		}
		else if (sym == "uinteger")
		{
			int num = number;
			if (num > amax)
			{
				error(15);
			}
			getsym();
			stringstream ss;
			ss << number;
			result = ss.str();
		}
		else if (sym == "lparen")
		{
			getsym();
			/*			int length_temp = sizeof(fsys) / sizeof(string);
			string fsys_temp[sizeof(fsys)/sizeof(string)+1];
			memcpy(fsys_temp, fsys, length_temp);
			fsys_temp[length_temp] = "rparen";
			*/			expression(result);
			if (sym == "rparen")
			{
				getsym();
			}
			else
			{
				error(3);
			}
		}
		test(continuesymbols, facbegsys, 41);
	}
}
void term(string &result)
{
	printf("now in term\n");
	string src1;
	string src2;
	string des;
	string opr;
	factor(src1);
	des = src1;
	while (sym == "times" || sym == "slash")
	{
		if (sym == "times")
			opr = "MUL";
		else
			opr = "DIV";
		des = generate_temp_var();
		getsym();
		factor(src2);
		generate(opr, src1, src2, des);
		src1 = des;
	}
	result = des;
}

void expression(string &result)
{
	/*	if (sym == "ident")
	{
	//	getsym();
	int pos = position(iden);
	if (pos == 0)
	{
	error(5);
	}
	}
	else
	error(99);*/
	string src1;
	string src2;
	string des;
	string opr;
	bool symbol_flag = false;
	printf("now in expression\n");
	if (sym == "plus" || sym == "minus")
	{
		if (sym == "minus")
			symbol_flag = true;
		getsym();
	}

	term(src1);
	if (symbol_flag == true)
	{
		des = generate_temp_var();
		generate("OPP", src1, "", des);
		src1 = des;
	}
	des = src1;
	while (sym == "plus" || sym == "minus")
	{
		if (sym == "plus")
			opr = "ADD";
		else
			opr = "SUB";
		des = generate_temp_var();
		getsym();
		term(src2);
		generate(opr, src1, src2, des);
		src1 = des;
	}
	result = des;
}
void condition()
{
	printf("now in condition\n");
	int i;
	string src1;
	string src2;
	string des;
	string opr;
	expression(src1);
	string compare_symbols[] = { "eql", "neq", "lss", "leq", "gtr", "geq", "" };
	string oprs[] = { "JNE", "JEQ", "JGE", "JGR", "JLE", "JLS" };
	if ((i = ifin(sym, compare_symbols)) != 0)
	{
		opr = oprs[i-1];
		getsym();
		expression(src2);
	}
	else
		error(22);
	generate(opr, src1, src2, "");
}
void statement()
{
	string correctsymbols[] = { "ident", "ifsym", "dosym","beginsym","readsym","writesym","forsym",""};
	string continuesymbols[] = { "semicolon","endsym","" };
	test(correctsymbols, continuesymbols, 39);
	printf("now in statement\n");
	if (sym == "ident")
	{
		string src1;
		string src2;
		string des;
		string opr;
		//判断是不是函数。若是的话，也没什么，卧槽函数部分好傻逼啊，难道是弱类型的语言吗，可以随便赋值。
		int pos = position(iden);

		if (pos == 0)
		{
			error(0);
		}
		if (id_table[pos].obj == "procedure")
		{
			des = id_table[pos].name;
			printf("now in call_statement\n");
			getsym();
			function_params(pos);
			generate("JMP","","",des);
			if (sym != "semicolon")
				getsym();
		}
		else
		{
			
			if (id_table[pos].type == "array")
			{
				src1 = id_table[pos].name;
				getsym();//是否对数组越界进行检查，，，，
				array_ident(src2);
			}
			else if (id_table[pos].type == "integersym" || id_table[pos].type == "charsym")
			{
				des = id_table[pos].name;
			}
			else
			{
				error(12);//此处需要报警
			}
			printf("now in assign_statement\n");
			getsym();
			if (sym == "becomes")
				getsym();
			else
				error(24);
			
			if (id_table[pos].type == "array")
			{
				expression(des);
				generate("STORE",src1,src2,des);
			}
			else if (id_table[pos].type == "integersym" || id_table[pos].type == "charsym")
			{
				expression(src1);
				generate("ASSIGN",src1,"",des);
			}
		}

	}
	else if (sym == "ifsym")
	{
		printf("now in if_statement\n");
		getsym();
		int code_index_temp = code_index;//这儿需要回填跳转符号。
		condition();
		if (sym == "thensym")
			getsym();
		else
			error(25);
		statement();
		int code_index_temp2 = code_index;
		generate("JMP", "", "", "");
		string label = generate_label();
		generate("LABEL", "", "", label);
		codes[code_index_temp].des = label;
		if (sym == "else")
			statement();
		string label2 = generate_label();
		generate("LABEL","","",label2);
		codes[code_index_temp2].des = label2;
	}
	else if (sym == "dosym")
	{
		printf("now in dowhilestatement\n");
		string label = generate_label();
		generate("LABEL", "", "", label);
		getsym();
		statement();
		if (sym == "whilesym")
			getsym();
		else
			error(26);
		int code_index_temp = code_index;
		condition();
		codes[code_index_temp].des = label;
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
					int pos = position(iden);

					if (pos == 0)
					{
						error(0);
					}

					generate("READ","","",iden);
				}
				else
					error(2);
				getsym();
			} while (sym == "comma");
		}
		else
			error(8);
		if (sym != "rparen")
			error(3);
		getsym();
	}
	else if (sym == "writesym")
	{
		printf("now in writestatement\n");
		getsym();
		string des;
		if (sym == "lparen")
		{
			getsym();
			if (sym == "string")//感觉string需要进行特殊处理啊！！！！！！！这儿需要回来再想！
			{

				getsym();
				if (sym == "comma")
				{
					getsym();
					expression(des);
				}
			}
			else
				expression(des);
			generate("WRITE","","",des);
			if (sym != "rparen")
				error(3);
			getsym();
		}
		else
			error(8);
	}
	else if (sym == "forsym")
	{
		printf("now in forstatement\n");
		getsym();
		string src1;
		string src2;
		string des;
		string opr;
		if (sym == "ident")
		{
			getsym();
			int pos = position(iden);
			if (pos == 0)
			{
				error(0);
			}
			des = iden;
		}
		else
			error(2);
		if (sym == "becomes")
		{
			getsym();
			expression(src1);
		}
		else
			error(24);
		generate("ASSIGN", src1, "", des);
		string label = generate_label();
		generate("LABEL", "", "", label);
		if (sym == "downtosym" || sym == "tosym")
		{
			if (sym == "downtosym")
				opr = "SUB";
			else
				opr = "ADD";
			getsym();
			expression(src2);
		}
		else
			error(27);

		if (sym == "dosym")
		{
			getsym();
			statement();
		}
		else
		{
			error(28);
		}
		generate(opr,des,"1",des);
		generate("JNE",des,src2,label);
	}
	string correctsymbols2[] = { "semicolon", "endsym", "whilesym","" };
	string continuesymbols2[] = { "" };
	test(correctsymbols2, continuesymbols2, 38);
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
		error(29);
	
}
