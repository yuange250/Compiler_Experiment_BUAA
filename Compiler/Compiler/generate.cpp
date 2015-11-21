#include "stdafx.h"
#include "globals.h"
void generate(string opr, string src1, string src2, string des)
{
	codes[code_index++].opr = opr;
	codes[code_index].src1 = src1;
	codes[code_index].src2 = src2;
	codes[code_index].des = des;
}
