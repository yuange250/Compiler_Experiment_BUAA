#include "stdafx.h"
#include "globals.h"
void getsym();
string errors[] = { "该标识符未定义", //0
"标识符重复定义",   //1
"应是标识符",   //2
"应是 )",//3
"应是 : ,在说明类型时必须有此冒号", //4
"非法符号，以跳读",  //5
"形式参数表中,形参说明应该以标识符或var开头", //6
"应是of",//7
"应是( ",//8
"类型定义必须以标识符打头",//9
"应是 [ ",//10
"应是 ] ",//11
"应为char或者integer",//12
"应是 = , := 只能用于赋值语句",//13
"缺少条件语句",//14
"数值太大,应小于2**32",//15
"程序结尾应该为.",//16
"字符中含有非法字符",//17
"应是类型标示符",//18
"字符串字符错误",//19
"应为 ( ",//20
"函数或过程传入参数数量不正确",//21
"应为判断符,>=,>,<>...",//22
"我就不信这个错误会被爆出来！",//23
"应为赋值号 := ",//24
"应该是then",//25
"应为while",//26
"应为downto或者to",//27
"应为do",//28
"应为end",//29
"下雪了,分程序怎么能没有复合语句",//30
"常量的值应该是字符或者数字",//31
"应是 =",//32
"应为 ;",//33
"应为 : ",//34
"数组下标错误",//35
"因子开始错误，应为标识符，数字或者左括号",//36
"应为声明符或者begin",//37
"错误的语句结束符",//38
"错误的语句开始符",//39
"声明部分错误的结束符",//40
"因子结束错误",//41
"类型匹配错误，请检查",//42
"函数赋值错误",//43
"实参传入错误",//44
"常量不可以赋值",//45
"传地址不可以传常量"//46
};
int ifin(string symbl, string * symbs)
{
	for (int i=0; symbs[i] != "";i++)
		if (symbl == symbs[i])
			return i+1;
	return 0;
}

void error(int error_no)
{
	//print out 
	printf("错误:在%d行%d字符处，%s\n",ln,cc,errors[error_no].c_str());
	err++;
//	printf("error occur!\n");
}

void test(string *s1, string *s2, int error_no)
{
	if (!ifin(sym, s1))
	{
		error(error_no);
		while (!(ifin(sym, s1) || ifin(sym, s2)))
			getsym();
	}
}

int position(string id){
	int i = 0;
	id_table[0].name = id;
	i = tx;
	while (i!=0)
	{
		if (id_table[i].name == id&&id_table[i].able)
			break;
		i = i - 1;
	}
	return i;
}
int check_var_ifexist(string id,int level_temp)
{
	int i = 0;
	id_table[0].name = id;
	i = tx;
	while (id_table[i].name != id&&i != 0)
	{
	
		if (id_table[i].lev != level_temp)
		{
			if (id_table[i].name == id)
				return i;
			return 0;
		}
		i = i - 1;
	}
	return i;
}
int check_func_assign_ifOK(string id)
{
	int i = 0;
	id_table[0].name = id;
	i = tx;
	while (id_table[i].lev != (level-1)&&i!=0)
	{
		i--;
	}
	if (id_table[i].name == id&&i!=0)
		return 1;
	else
		return 0;
}
int find_func_proc_position(string label)
{
	string name;
	string code_temp;
	int code;
	int i;
	for (i = 0; i < label.size(); i++)
	{
		char c = label.at(i);
		if (c != '_')
		{
			name.append(1,c);
		}
		else
			break;
	}
	for (i++; i < label.size(); i++)
	{
		code_temp.append(1, label.at(i));
	}
	code = atoi(code_temp.c_str());
	for (int i = 1; i <= tx; i++)
	{
		if ((id_table[i].obj == "function" || id_table[i].obj == "procedure") &&id_table[i].param_list->function_code == code)
		{
			return i;
		}
	}
	return 0;
}
int get_the_variable(int posi,string name)
{
	//一层层地向外跳着找变量，很笨的办法。
	for (int j = posi,level=id_table[posi].lev+1; id_table[j].obj == "procedure" || id_table[j].obj == "function";)
	{
		for (int i = j+1; i <= tx && (id_table[i].obj == "var" || id_table[i].obj == "const"); i++)
		{
			if (id_table[i].name == name)
			{
				return i;
			}
		}
		level--;
		while (id_table[j].lev >= level&&j>0&&level>0)
			j--;
	}
}
string int_to_string(int num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}