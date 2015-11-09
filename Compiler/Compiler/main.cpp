// Compiler.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include "globals_const.h"
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
using namespace std;
//Global Value
string symbols[] = {"nul","ident","uinteger","integer","char","string","plus","minus","times","slash","oddsym","eql","neq","lss","leq","gtr","geq","lparen","rparen","comma","semicolon","colon","period","becomes","beginsym","endsym","ifsym","thensym","whilesym","dosym","forsym","downsym","tosym","callsym","constsym","varsym","procsym","readsym","writesym","funcsym"};
string identtype[] = {"constant","variable","procedure","function"};
char ch=' ';
string sym;//last symbol read
string iden;//last identifier read
int number;//last number read
int cc=0;//charactor count
int ll=0;//linelen
int err = 0;//err_nums

string a;
char line[line_max] = {0};
string reser_word[] = {"beigin","call","const","do","end","if","odd","procedure","read","then","var","while","write","for","down","to","function","integer","char"};
string wsym[] = { "beginsym","callsym", "constsym", "dosym", "endsym", "ifsym", "oddsym", "procsym", "readsym", "thensym", "varsym","whilesym" ,"writesym","forsym","downsym","tosym","funcsym","intsym","charsym"};//word symbols
map<char,string> ssym;//+ - * / ( ) < >
struct table{
	string name;
	string kind;
	int value;
	int level;
	int addr;
};
int tx = 0;//table index
table id_table[txmax];
void init_ssym(){
	ssym['+'] = "plus";        
	ssym['-'] = "minus";
	ssym['*'] = "times";       
	ssym['/'] = "slash";
	ssym['('] = "lparen";  
	ssym[')'] = "rparen";
	ssym['='] = "eql";         
	ssym[','] = "comma";
	ssym['.'] = "period";
	ssym['<'] = "lss";         
	ssym['>'] = "gtr";
	ssym[';'] = "semicolon";
	ssym[':'] = "colon";
}
string declbegsys[] = { "constsym", "varsym", "procsym" };
string statbegsys[] = { "beginsym", "callsym", "ifsym", "whilesym" };
string facbegsys[] = { "ident", "uinteger","lparen" };
FILE *IN, *OUT;
int main(int argc, char**argv)
{
	init_ssym();
	printf("%d", sizeof(facbegsys)/sizeof(string));
	getchar();
	return 0;
}

