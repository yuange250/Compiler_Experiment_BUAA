#include "stdafx.h"
#include "globals.h"

#include<stdio.h>
void error(int error_no);
void getch()
{
	if (cc == ll)
	{
		ln++;
		char c_temp = 0;
		if ((c_temp=fgetc(IN))==EOF)
		{
			error(2);
			fclose(IN);
		}
		ungetc(c_temp,IN);
		ll = 0;
		cc = 0;
		c_temp = 0;
		while (c_temp != '\n')
		{
			c_temp = fgetc(IN);
			if (c_temp!='\n')
				line[ll++] = c_temp;
		}
		line[ll++] = ' ';//加了一个缓冲，用于回退
	}
	ch = line[cc];
	cc++;
}
void ugetch()
{
	cc--;
	ch = line[cc];
	cc++;
}
void getsym()
{
	//需要向前看啊，我觉得最好能提前预知下一个是什么。。。。
	//sym = nextsym;
	while (ch == ' '||ch=='\t')
	{
		getch();
	}
	if (ch >= 'a'&&ch <= 'z' || (ch >= 'A'&&ch <= 'Z'))
	{
		int i = 0;
		a = "";
		while ((ch >= 'a'&&ch <= 'z') || (ch >= '0'&&ch <= '9') || (ch >= 'A'&&ch <= 'Z'))
		{
			if (i > al)
			{

			}
			a.append(1, ch);
			getch();
		}
		//	a.append(1,'\0');
		iden = a;
		i = 0;
		for (i = 0; i < rwnu; i++)
		{
			if (iden == reser_word[i])
			{
				break;
			}
		}
		if (i == rwnu)
		{
			sym = "ident";
		}
		else
		{
			sym = wsym[i];
		}
//		cout<<sym+","<<iden<<endl;
	}
	else if (ch >= '0'&&ch <= '9')
	{
		int i = 0;
//		int point_flag = 0;
		number = ch-'0';
		getch();
		while (ch >= '0'&&ch <= '9')
		{
//			if (ch == '.')
//				point_flag = 1;//有了实数之后，遇到一个难题，程序结尾也有一个句号，，，，，吐血
			
			number = 10 * number + (ch - '0');
			getch();
		}
		//			if (point_flag==0)
		sym = "uinteger";
//		cout << sym + "," << number << endl;
//		else
//			sym = "ureal";
	}
	else if (ch==':')
	{
		getch();
		if (ch == '=')
		{
			sym = "becomes";
//			cout << sym + "," << ":=" << endl;
			getch();
		}
		else
		{
			sym = "colon";//这儿要注意回来看一下,好了，不用看
//			cout << sym + "," << ":"<< endl;
		}
	}
	else if (ch == '<')
	{
		getch();
		if (ch == '=')
		{
			sym = "leq";
//			cout << sym + "," << "<=" << endl;
			getch();
		}
		else if (ch == '>')
		{
			sym = "neq";
//			cout << sym + "," << "<>" << endl;
			getch();
		}
		else
		{
			sym = "lss";
//			cout << sym + "," << "<" << endl;
		}
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = "geq";
//			cout << sym + "," << ">=" << endl;
			getch();
		}
		else
		{
			sym = "gtr";
//			cout << sym + "," << ">" << endl;
		}
	}
	else if (ch=='\'')
	{
		getch();
		int i = 0;
		a = "";
		if ((ch >= 'a'&&ch <= 'z') || (ch >= '0'&&ch <= '9')||(ch>='A'&&ch<='Z'))
		{
			a.append(1, ch);
			getch();
			if (ch == '\'')
			{
				a.append(1, '\0');
				sym = "char";
				iden = a;
			}
			else
			{
				sym = "nul";
				iden = "";
				error(17);
				while (ch != '\'')
				{
					getch();
				}

			}
		}
		else
		{
			sym = "nul";
			iden = "";
			error(17);
			while (ch != '\'')
			{
				getch();
			}
		}
		getch();
//		cout << sym + "," << iden << endl;
	}
	else if (ch == '\"')
	{
		getch();
		int i = 0;
		a = "";
		while (ch != '\"')
		{
			if (ch==32||ch==33||(ch>=35&&ch<=126))//这儿应该是合法字符，卧槽，合法是个什么定义！
			{
				a.append(1,ch);
			}
			else
			{
				error(23);
			}
			getch();
		}
		a.append(1,'\0');
		iden = a;
		sym = "string";
//		cout << sym + "," << iden << endl;
		getch();
	}
	else
	{
		sym= ssym[ch];
		if (sym == "")
		{
			error(9);//illegal charactor
		}
		else
//		  cout << sym + "," << ch << endl;
		getch();
	}
}
