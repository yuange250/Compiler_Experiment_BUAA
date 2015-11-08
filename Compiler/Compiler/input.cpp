#include "stdafx.h"
#include "globals.h"

#include<stdio.h>
void error(int error_no);
void getch()
{
	if (cc == ll)
	{
		char c_temp = 0;
		if ((c_temp=fgetc(IN))==EOF)
		{
			fclose(IN);
		}
		ungetc(c_temp,IN);
		ll = 0;
		cc = 0;
		c_temp = 0;
		while (c_temp != '\n' && c_temp != '.')
		{
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
	while (ch == ' ')
	{
		getch();
	}
	if (ch >= 'a'&&ch <= 'z')
	{
		int i = 0;
		a = "";
		while ((ch >= 'a'&&ch <= 'z')||(ch>='0'&&ch<='9'))
		{
			if (i > al)
			{

			}
			a.append(1,ch);
			getch();
		}
		a.append(1,'\0');
		iden = a;
		i = 0;
		for (i = 0; i < rwnu; i++)
		{
			if (iden == reser_word[i])
			{
				break;
			}
		}
		if (i != rwnu)
		{
			sym = "ident";
		}
		else
		{
			sym = wsym[i];
		}
	}
	else if (ch == '-' || ch == '+')
	{
		char c_temp = ch;
		number = 0;
		getch();
		if (ch >= '0'&&ch <= '9')
		{
			int i = 0;
//			int point_flag = 0;
			while (ch >= '0'&&ch <= '9')
			{
//				if (ch == '.')
//					point_flag = 1;
				number = 10 * number + (ch - '0');
				getch();
			}
//			if (point_flag==0)
			sym = "integer";
			if (c_temp == '-')
			{
				number = -number;
			}
//			else
//				sym = "real";
		}
		else
		{
			sym = ssym[c_temp];
		}
	}
	else if (ch >= '0'&&ch <= '9')
	{
		int i = 0;
//		int point_flag = 0;
		number = 0;
		while (ch >= '0'&&ch <= '9')
		{
//			if (ch == '.')
//				point_flag = 1;//有了实数之后，遇到一个难题，程序结尾也有一个句号，，，，，吐血
			
			number = 10 * number + (ch - '0');
			getch();
		}
		//			if (point_flag==0)
		sym = "uinteger";
//		else
//			sym = "ureal";
	}
	else if (ch==':')
	{
		getch();
		if (ch == '=')
		{
			sym = "becomes";
			getch();
		}
		else
			sym = "nul";//这儿要注意回来看一下,好了，不用看了
	}
	else if (ch == '<')
	{
		getch();
		if (ch == '=')
		{
			sym = "leq";
			getch();
		}
		else if (ch == '>')
		{
			sym = "neq";
			getch();
		}
		else
			sym = "lss";
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = "geq";
			getch();
		}
		else
			sym = "gtr";
	}
	else if (ch=='\'')
	{
		getch();
		int i = 0;
		a = "";
		if ((ch >= 'a'&&ch <= 'z') || (ch >= '0'&&ch <= '9'))
		{
			a.append(1, ch);
			getch();
			if (ch == '\'')
			{
				a.append(1, '\0');
				sym = "char";
				getch();
			}
			else
			{
				sym = "nul";
				error(1);
			}
		}
		else
		{
			sym = "nul";
			error(1);
		}
	}
	else if (ch == '\"')
	{
		getch();
		int i = 0;
		a = "";
		while (ch != '\"')
		{
			if (ch)//这儿应该是合法字符，卧槽，合法是个什么定义！
			{
				a.append(1,ch);
			}
			else
			{
				error(1);
				break;
			}
		}
		a.append(1,'\0');
		if (ch == '\"')
		{
			sym = "string";
			getch();
		}
		else
		{
			error(1);
			sym = "nul";
		}
	}
	else
	{
		sym= ssym[ch];
		getch();
	}
}
