#include "globals.h"
#include<stdio.h>
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
		while ((ch >= 'a'&&ch <= 'z')||(ch>='0'&&ch<='9'))
		{
			if (i > al)
			{

			}
			a[i++] = ch;
			getch();
		}
		iden = a;
		int i = 0;
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
			sym = reser_word[i];
		}
	}
	else if (ch == '-' || ch == '+')
	{
		char c_temp = ch;
		getch();
		if (ch >= '0'&&ch <= '9')
		{
			int i = 0;
			int point_flag = 0;
			while (ch >= '0'&&ch <= '9' || (point_flag == 0 && ch == '.'))
			{
				if (ch == '.')
					point_flag = 1;
				number[i++] = ch;
				getch();
			}
			if (point_flag==0)
			{
				sym = "integer";
			}
			else
			{
				sym = "real";
			}
		}
		else
		{
			sym = ssym[c_temp];
		}
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
	else
	{
		sym= ssym[ch];
		getch();
	}
}