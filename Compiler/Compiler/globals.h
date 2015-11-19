#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include "globals_const.h"
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
	int param_num;
	string param_names[param_max];
	string types[param_max];
};
struct table{
	string name;
	string obj;
	string type;
	bool able;//符号的使能键,好吧，现在我发现这玩意没什么鸡巴用
	int lev;
	int adr;//地址,这个就叼啦
	int value;//其实一般是没什么用的，毕竟最后弄成汇编
	struct params * param_list;
	struct array_info * arrayinfo;
};
extern params PARAMS;
extern array_info ARRAY_INFO;
extern table id_table[];
extern int tx;//table index
extern int level;//levelѽ