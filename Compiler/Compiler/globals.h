#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include "globals_const.h"
#include <sstream>
using namespace std;
extern string symbols[];
extern char ch;
extern string sym;//last symbol read
//extern string nextsym;//yes for look forward
extern string iden;//last identifier read
extern int number;//last number read
extern int cc;//charactor count
extern int ll;//linelen
extern int err;//nums of err
extern string a;
extern char line[];
extern string reser_word[];
extern string wsym[];//word symbols
extern map<char, string> ssym;//+ - * / ( ) < >
extern string declbegsys[];
extern string statbegsys[];
extern string facbegsys[];
extern FILE *IN, *OUT;
struct array_info{
	int size;
	string type;
};
struct params{
	int function_code;
	int param_num;
	string param_names[param_max];
	string types[param_max];
	bool addr_or_value[param_max];
};
struct table{
	string name;
	string obj;
	string type;
	bool able;//使能键
	int lev;
	int adr;//地址,这个就叼啦
	int value;//其实一般是没什么用的，毕竟最后弄成汇编
	struct params * param_list;
	struct array_info * arrayinfo;
	bool addr_or_value;
};
extern params PARAMS;
extern array_info ARRAY_INFO;
extern table id_table[];
extern int tx;//table index
extern int level;//levelѽ
extern int sp_addr;
extern int sp_piece_top;
struct code{
	string opr;
	string src1;
	string src2;
	string des;
};
extern code MID_CODE;
extern code codes[codes_max];
extern int code_index;
extern int temp_var_num;
extern int label_num;
extern int ln;//linenum
extern int function_num;
extern int string_num;
extern string string_pool[100];