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
extern string nextsym;//yes for look forward
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
struct table{
	string name;
	string kind;
	int value;
	int level;
	int addr;
};
extern table id_table[txmax];
extern int tx;//table index