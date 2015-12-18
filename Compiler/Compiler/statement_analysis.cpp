//line348 打印string需要继续细想，可能需要放到.data中！
#include "stdafx.h"
#include "globals.h"
void error(int error_no);
int position(string id);
void test(string *s1, string *s2, int error_no);
int ifin(string symbol, string *symbols);
void getsym();
void expression(string &result, string &result_type);
void multi_statement();
string generate_temp_var();
void generate(string opr, string src1, string src2, string des);
int check_var_ifexist(string id, int lev);
int check_func_assign_ifOK(string id);
string generate_label();
string generate_func_proc_label(string name, int code);
void array_ident(string &result);
string generate_string_Label(int string_num_);
void function_params(int posi){//这个也是废弃的矿坑,好吧，这不是
	string result;
	string type;
	string push_result[10];
	string off_site[10];
	int addr_or_value[10];
	int param_num = 0;
	if (sym == "lparen")
	{
		int nums = 0;
		do{
			getsym();
			if (id_table[posi].param_list->addr_or_value[nums])
			{
				if (sym == "ident")
				{
					int positi=position(iden);
					if (positi == 0)
					{
						error(0);
						expression(result, type);
					}
					else if (id_table[positi].obj == "const")
					{
						error(46);
						getsym();
					}
					else if (id_table[positi].type == "integersym" || id_table[positi].type == "charsym")
					{
				//		generate("PUSHA", id_table[positi].name, "0", "");
						addr_or_value[param_num] = 1;
						off_site[param_num] = "0";
						push_result[param_num++] = id_table[positi].name;

						type = id_table[positi].type;
						getsym();
					}
					else if (id_table[positi].type == "array")
					{
						string array_name = iden;
						string array_offside;
						type = id_table[positi].arrayinfo->type;
						getsym();
						array_ident(array_offside);
				//		generate("PUSHA", array_name, array_offside, "");
						addr_or_value[param_num] = 1;
						off_site[param_num] = array_offside;
						push_result[param_num++] = array_name;
						getsym();
					}
					else
					{
						error(44);
					}
				}
				else
				{
					error(2);
					expression(result, type);
				}
			}
			else
			{
				expression(result, type);
				addr_or_value[param_num] = 0;
				off_site[param_num] = "";
				push_result[param_num++] = result;
	//			generate("PUSH",result,"","");
			}
			if (id_table[posi].param_list->types[nums] == "charsym"&&type != "charsym")
				error(42);
			nums++;
		} while (sym == "comma");
		
		if (sym != "rparen")
		{
			error(3);
		}
		else
			getsym();
		if (nums != id_table[posi].param_list->param_num)
		{
			error(21);
		}
		
		//int paramnum;//参数个数。
	}
	generate("CALL", generate_func_proc_label(id_table[posi].name, id_table[posi].param_list->function_code), "", "");
	for (int i = 0; i < param_num; i++)
	{
		if (addr_or_value[i])
			generate("PUSHA", push_result[i], off_site[i], "");
		else
			generate("PUSH", push_result[i], "", "");
	}
	generate("JMP", "", "", generate_func_proc_label(id_table[posi].name, id_table[posi].param_list->function_code));
}
void array_ident(string &result)
{
	string type;
	if (sym != "lbracket")
	{
		error(10);
	}
	getsym();
	expression(result,type);
	if (sym != "rbracket")
	{
		error(11);
	}
}
void factor(string &result, string &result_type){
//	printf("now in factor\n");
	int i = 0;
	//	test(facbegsys,fsys,24);
	//string correctsymbols[] = { "ident", "ifsym", "dosym", "beginsym", "readsym", "writesym", "forsym", "" };
	string continuesymbols[] = { "semicolon", "endsym", "times", "plus", "slash", "minus", "eql", "neq", "lss", "leq", "gtr", "geq", "thensym","tosym","downtosym","rparen","rbracket","dosym","whilesym","comma","elsesym","" };
	test(facbegsys, continuesymbols, 36);
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
					string des = generate_temp_var();
					generate("ASSIGN", "@", "", des);
					result = des;
					result_type = id_table[posi].type;
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
					result_type = id_table[posi].arrayinfo->type;
					getsym();
				}
				else
				{
					result = iden;
					result_type = id_table[posi].type;
					getsym();
				}
			}
			
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
			result_type = "integersym";
		}
		else if (sym == "lparen")
		{
			getsym();
			/*			int length_temp = sizeof(fsys) / sizeof(string);
			string fsys_temp[sizeof(fsys)/sizeof(string)+1];
			memcpy(fsys_temp, fsys, length_temp);
			fsys_temp[length_temp] = "rparen";
			*/			
			expression(result,result_type);
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
void term(string &result,string &result_type)
{
//	printf("now in term\n");
	string src1;
	string src2;
	string des;
	string opr;
	string type;
	bool type_sure_flag = false;
	factor(src1,type);
	des = src1;
	if (type == "integersym")
	{
		result_type = type;     //只要有一个因素的类型是int那么整个term的类型就是int
		type_sure_flag = true;
	}
	while (sym == "times" || sym == "slash")
	{
		if (sym == "times")
			opr = "MUL";
		else
			opr = "DIV";
		des = generate_temp_var();
		getsym();
		factor(src2,type);
		if (type == "integersym")
		{
			result_type = type;
			type_sure_flag = true;
		}
		generate(opr, src1, src2, des);
		src1 = des;
	}
	result = des;
	if (!type_sure_flag)
	{
		result_type = "charsym";
	}
}

void expression(string &result,string &result_type)
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
	string type;
	bool symbol_flag = false;
	bool type_sure_flag = false;

//	printf("now in expression\n");
	if (sym == "plus" || sym == "minus")
	{
		if (sym == "minus")
			symbol_flag = true;
		getsym();
	}

	term(src1,type);
	if (type == "integersym")
	{
		result_type = type;     //只要有一个因素的类型是int那么整个term的类型就是int
		type_sure_flag = true;
	}
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
		term(src2,type);
		if (type == "integersym")
		{
			result_type = type;     //只要有一个因素的类型是int那么整个term的类型就是int
			type_sure_flag = true;
		}
		generate(opr, src1, src2, des);
		src1 = des;
	}
	result = des;
	if (!type_sure_flag)
	{
		result_type = "charsym";
	}
}
int condition(int flag)
{
//	printf("now in condition\n");
	int i;
	string src1;
	string src2;
	string des;
	string opr;
	string type;
	expression(src1,type);//我还是觉得条件语句并不需要检查类型的问题,'1'>'2'也是阔以的嘛
	string compare_symbols[] = { "eql", "neq", "lss", "leq", "gtr", "geq", "" };
	string oprs[] = { "JNE", "JEQ", "JGE", "JGR", "JLE", "JLS" };
	string oprs2[] = { "JEQ", "JNE", "JLS", "JLE", "JGR", "JGE", "" };
	if ((i = ifin(sym, compare_symbols)) != 0)
	{
		if (flag == 0)
			opr = oprs[i - 1];
		else
			opr = oprs2[i-1];
		getsym();
		expression(src2,type);
	}
	else
		error(22);
	generate(opr, src1, src2, "");
	return code_index - 1;
}
void statement()
{
	string correctsymbols[] = { "ident", "ifsym", "dosym", "beginsym", "readsym", "writesym", "forsym", "semicolon", "endsym","" };
	string continuesymbols[] = { "" };
	test(correctsymbols, continuesymbols, 39);
//	printf("now in statement\n");
	if (sym == "ident")
	{
		string src1;
		string src2;
		string des;
		string opr;
		string type;
		//判断是不是函数。若是的话，也没什么，卧槽函数部分好傻逼啊，难道是弱类型的语言吗，可以随便赋值。
		int pos = position(iden);

		if (pos == 0)
		{
			error(0);
		}
		if (id_table[pos].obj == "procedure")
		{
			des = id_table[pos].name;
//			printf("now in call_statement\n");
//			generate("CALL", generate_func_proc_label(id_table[pos].name, id_table[pos].param_list->function_code), "", "");
			getsym();
			function_params(pos);
		
		/*	if (sym != "semicolon")
				getsym();*/
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
				if (id_table[pos].obj == "const")
				{
					error(45);
				}
				des = id_table[pos].name;
				if (id_table[pos].obj=="function"&&!check_func_assign_ifOK(des))
				{
					error(43);
				}
			}
			else
			{
				error(12);//此处需要报警
			}
	//		printf("now in assign_statement\n");
			getsym();
			if (sym == "becomes")
				getsym();
			else
				error(24);
//			string continuesymbols2[] = { "" };
//			test(facbegsys, continuesymbols2, 36);
			if (id_table[pos].type == "array")
			{
				expression(des,type);
				if (id_table[pos].arrayinfo->type == "charsym"&&type != "charsym")
					error(42);
				generate("STORE",src1,src2,des);
			}
			else if (id_table[pos].type == "integersym" || id_table[pos].type == "charsym")
			{
				expression(src1,type);
				if (id_table[pos].type == "charsym"&&type != "charsym")
					error(42);
				generate("ASSIGN",src1,"",des);
			}
		}

	}
	else if (sym == "ifsym")
	{
//		printf("now in if_statement\n");
		getsym();
		int code_index_temp = condition(0);//这儿需要回填跳转符号。
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
		if (sym == "elsesym")
		{
			getsym();
			statement();
		}
		string label2 = generate_label();
		generate("LABEL","","",label2);
		codes[code_index_temp2].des = label2;
	}
	else if (sym == "dosym")
	{
//		printf("now in dowhilestatement\n");
		string label = generate_label();
		generate("LABEL", "", "", label);
		getsym();
		statement();
		if (sym == "whilesym")
			getsym();
		else
			error(26);
		int code_index_temp=condition(1);
		codes[code_index_temp].des = label;
	}
	else if (sym == "beginsym")
	{
//		printf("now in begin_statement\n");
		multi_statement();
	}
	else if (sym == "readsym")
	{
//		printf("now in readstatement\n");
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

					generate("READ","",id_table[pos].type,iden);
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
//		printf("now in writestatement\n");
		getsym();
		string des;
		string type;
		if (sym == "lparen")
		{
			getsym();
			if (sym == "string")//感觉string需要进行特殊处理啊！！！！！！！这儿需要回来再想！
			{
				string_pool[string_num] = iden;
				generate("WRITE", "", "string", generate_string_Label(string_num));
				string_num++;
				getsym();
				if (sym == "comma")
				{
					getsym();
					expression(des,type);
					generate("WRITE", "", type, des);
				}
			}
			else
			{
				expression(des, type);//这儿也是不需要滴！
				generate("WRITE", "", type, des);
			}
			if (sym != "rparen")
				error(3);
			getsym();
		}
		else
			error(8);
	}
	else if (sym == "forsym")
	{
//		printf("now in forstatement\n");
		getsym();
		string src1;
		string src2;
		string des;
		string opr;
		string type;
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
			expression(src1,type);
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
			expression(src2,type);
		}
		else
			error(27);
		int code_index_temp = code_index;
		if (opr=="ADD")
			generate("JGR", des, src2, "");
		else
			generate("JLS", des, src2, "");
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
		generate("JMP","","",label);
		string label2 = generate_label();
		generate("LABEL", "", "", label2);
		codes[code_index_temp].des = label2;
	}
	string correctsymbols2[] = { "semicolon", "endsym", "whilesym","elsesym","" };
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
