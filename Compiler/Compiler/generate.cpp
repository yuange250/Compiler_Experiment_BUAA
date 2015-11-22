#include "stdafx.h"
#include "globals.h"
void generate(string opr, string src1, string src2, string des)
{
<<<<<<< HEAD
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
	return "T" + ss.str();
}
string generate_label()
{
	stringstream ss;
	ss << label_num;
	label_num++;
	return "Label" + ss.str();
}
void listcode()
{
	ofstream fout("mid_code.txt");
	for (int i = 0; i < code_index; i++)
	{
		fout<<codes[i].opr<<" "<<codes[i].src1<<" "<<codes[i].src2<<" "<<codes[i].des<<endl;
	}
}
=======
	codes[code_index++].opr = opr;
	codes[code_index].src1 = src1;
	codes[code_index].src2 = src2;
	codes[code_index].des = des;
}
>>>>>>> origin/master
