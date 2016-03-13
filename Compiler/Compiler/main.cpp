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
string symbols[] = { "nul", "ident", "uinteger", "integer", "char", "string", "plus", "minus", "times", "slash", "oddsym", "eql", "neq", "lss", "leq", "gtr", "geq", "lparen", "rparen", "comma", "semicolon", "colon", "period", "becomes", "beginsym", "endsym", "ifsym", "thensym", "whilesym", "dosym", "forsym", "downsym", "tosym", "callsym", "constsym", "varsym", "procsym", "readsym", "writesym", "funcsym" };
string identtype[] = { "constant", "variable", "procedure", "function" };
char ch = ' ';
string sym;//last symbol read
//string nextsym="";
string iden;//last identifier read
int number;//last number read
int cc = 0;//charactor count
int ll = 0;//linelen
int ln = 0;//linenum
int err = 0;//err_nums

int level = 0;//level呀
int sp_addr = 0;//整个栈空间的地址交给这位
int sp_piece_top = 0;
int code_index = 0;

int temp_var_num = 0;
int label_num = 0;//标签命名，这个应该整个程序唯一。
int function_num = 1;
int string_num = 0;


string string_pool[100];

string a;
char line[line_max] = { 0 };
string reser_word[] = { "begin", "call", "const", "do", "end", "if", "odd", "procedure", "read", "then", "var", "while", "write", "for", "downto", "to", "function", "integer", "uinteger", "char", "of", "array","else","" };
string wsym[] = { "beginsym", "callsym", "constsym", "dosym", "endsym", "ifsym", "oddsym", "procsym", "readsym", "thensym", "varsym", "whilesym", "writesym", "forsym", "downtosym", "tosym", "funcsym", "integersym", "uintegersym", "charsym", "ofsym", "arraysym","elsesym","" };//word symbols
map<char, string> ssym;//+ - * / ( ) < >
int tx = 0;//table index
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
struct code{
	string opr;
	string src1;
	string src2;
	string des;
};
code MID_CODE;
params PARAMS;
array_info ARRAY_INFO;
code codes[codes_max];
table id_table[txmax];
void init(){
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
	id_table[++tx].lev = 0;
	id_table[tx].name = "MAIN";
	id_table[tx].param_list = (struct params*)malloc(sizeof(PARAMS));
	id_table[tx].param_list->function_code = 0;
	id_table[tx].obj = "procedure";
}
string declbegsys[] = { "constsym", "varsym", "procsym","" };
string statbegsys[] = { "beginsym", "callsym", "ifsym", "whilesym","" };
string facbegsys[] = { "ident", "uinteger", "lparen", "" };
FILE *IN, *OUT;
void getsym();
void block(string func_name, int code);
bool ifin(string symbol, string symbols[]);
void listcode();
void generate(string opr, string src1, string src2, string des);
void generatemips();
void generatemips_new();
void list_mips_code();
void listcode2();
void DAG_optimize();
void sight_optimize();
void active_var_analysis();
int main(int argc, char**argv)
{
	/*
	map<int,char> vars_map;

	for (int i = 0; i < 10; i++)
	{
		vars_map[i] = i + 'a';
	}
	
	for (map<int, char>::iterator it = vars_map.begin(); it != vars_map.end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}
	map<int, char>::iterator it2 = vars_map.find(10);
	if (it2==vars_map.end())
	{
		cout << "yes"<< endl;
	}
	*/
	char path[200];
	scanf("%s", path);
    IN = fopen(path, "r");
	init();
	getsym();
	block("MAIN",0);
//	generate("RETURN","","","");
	
	sight_optimize();
	

	DAG_optimize();
	//listcode();
	active_var_analysis();
	//listcode2();
	generatemips_new();
	//generatemips();
	
	list_mips_code();
	printf("Analysis over!");
	getchar();
	return 0;
	
}

