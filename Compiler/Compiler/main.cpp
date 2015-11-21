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
//string nextsym="";
string iden;//last identifier read
int number;//last number read
int cc=0;//charactor count
int ll=0;//linelen
int err = 0;//err_nums

int level = 0;//level呀
int sp_addr = 0;//整个栈空间的地址交给这位
int sp_piece_top = 0;
int code_index = 0;
string a;
char line[line_max] = {0};
string reser_word[] = {"begin","call","const","do","end","if","odd","procedure","read","then","var","while","write","for","down","to","function","integer","uinteger","char","of","array"};
string wsym[] = { "beginsym","callsym", "constsym", "dosym", "endsym", "ifsym", "oddsym", "procsym", "readsym", "thensym", "varsym","whilesym" ,"writesym","forsym","downsym","tosym","funcsym","integersym","uintegersym","charsym","ofsym","arraysym"};//word symbols
map<char,string> ssym;//+ - * / ( ) < >
int tx = 0;//table index
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
struct mid_code{
	string opr;
	string src1;
	string src2;
	string des;
};
mid_code MID_CODE;
params PARAMS;
array_info ARRAY_INFO;
mid_code codes[codes_max];
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
	ssym['['] = "lbracket";
	ssym[']'] = "rbracket";
}
string declbegsys[] = { "constsym", "varsym", "procsym" };
string statbegsys[] = { "beginsym", "callsym", "ifsym", "whilesym" };
string facbegsys[] = { "ident", "uinteger","lparen","" };
FILE *IN, *OUT;
void getsym();
void block();
bool ifin(string symbol, string symbols[]);
int main(int argc, char**argv)
{
	IN = fopen("in.pas","r");
	init_ssym();
	//printf("%d", sizeof(facbegsys)/sizeof(string));
	/*while (sym != "period")
	{
		getsym();
	}*/
//	sym = "const";
//	printf("%d", sym == reser_word[2]);
//	cout << sym == reser_word[2];
	getsym();

	block();
/*	string compare_symbols[] = { "eql", "neq", "lss", "leq", "gtr", "geq" };
	if (ifin("lss", compare_symbols))
	{
		printf("soga");
	}*/
	getchar();
	return 0;
}

