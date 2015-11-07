// Compiler.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
using namespace std;
//Global Value
const int rwnu = 13;//reserve word number
const int txmax = 100;//identifier table max num
const int nmax = 14;//max digit in numbers
const int al = 10;//max char num of a identifier
const int addmax = 2047;//address max
const int levmax = 3;//max depth of block
const int codemax = 200;//max line in a program's pcode
const int line_max = 81;
string symbols[] = {"nul","ident","uinteger","ureal","integer","real","char","string","plus","minus","times","slash","oddsym","eql","neq","lss","leq","gtr","geq","lparen","rparen","comma","semicolon","period","becomes","beginsym","endsym","ifsym","thensym","whilesym","dosym","callsym","constsym","varsym","procsym","readsym","writesym","funcsym"};
string identtype[] = {"constant","variable","procedure","function"};
char ch=' ';
string sym;//last symbol read
string iden;//last identifier read
string number;//last number read
int cc=0;//charactor count
int ll=0;//linelen
string a;
char line[line_max] = {0};
string reser_word[] = {"beigin","call","const","do","end","if","odd","procedure","read","then","var","while","write"};
string wsym[] = { "beginsym", "endsym", "ifsym", "thensym", "whilesym", "dosym", "callsym", "constsym", "varsym", "procsym", "readsym", "writesym" ,"oddsym"};//word symbols
map<char,string> ssym;//+ - * / ( ) < >
int test();
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
}
string declbegsys[] = { "constsym", "varsym", "procsym" };
string statbegsys[] = { "beginsym", "callsym", "ifsym", "whilesym" };
string facbegsys[] = { "ident", "uinteger","ureal","lparen" };
FILE *IN, *OUT;
int main(int argc, char**argv)
{
	init_ssym();
	test();
	getchar();
	return 0;
}

