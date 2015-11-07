#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
using namespace std;
extern const int rwnu;//reserve word number
extern const int txmax;//identifier table max num
extern const int nmax;//max digit in numbers
extern const int al;//max char num of a identifier
extern const int addmax;//address max
extern const int levmax;//max depth of block
extern const int codemax;//max line in a program's pcode
extern string symbols[];
extern char ch;
extern string sym;//last symbol read
extern string iden;//last identifier read
extern string number;//last number read
extern int cc;//charactor count
extern int ll;//linelen
extern string a;
extern char line[];
extern string reser_word[];
extern string wsym[];//word symbols
extern map<char, string> ssym;//+ - * / ( ) < >
extern string declbegsys[];
extern string statbegsys[];
extern string facbegsys[];
extern FILE *IN, *OUT;