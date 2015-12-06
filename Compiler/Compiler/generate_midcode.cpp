#include "stdafx.h"
#include "globals.h"
void generate(string opr, string src1, string src2, string des)
{
	codes[code_index].opr = opr;
	codes[code_index].src1 = src1;
	codes[code_index].src2 = src2;
	codes[code_index++].des = des;
}
string generate_temp_var()
{
	stringstream ss;
	ss << temp_var_num;
	temp_var_num++;
	return "%" + ss.str();
}
string generate_label()
{
	stringstream ss;
	ss << label_num;
	label_num++;
	return "_Label" + ss.str();
}
string generate_string_Label(int string_num_)
{
	stringstream ss;
	ss << string_num_;
	return "_Str_" + ss.str();
}
string generate_func_proc_label(string name,int code)
{
	stringstream ss;
	ss << code;
	string num = ss.str();
	return name + "_" + num;
}
void listcode()
{
	ofstream fout("mid_code.txt");
	for (int i = 0; i < code_index; i++)
	{
		fout<<codes[i].opr<<" "<<codes[i].src1<<" "<<codes[i].src2<<" "<<codes[i].des<<endl;
	}
}
void listcode2()
{
	ofstream fout("mid_code_after_optimize.txt");
	for (int i = 0; i < code_index; i++)
	{
		fout << codes[i].opr << " " << codes[i].src1 << " " << codes[i].src2 << " " << codes[i].des << endl;
	}
}
