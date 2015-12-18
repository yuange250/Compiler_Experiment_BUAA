#include "stdafx.h"
#include "globals.h"
void sight_optimize()
{
	for (int i = 0; i < code_index; )
	{
		if (codes[i].opr == "ASSIGN")
		{
			if (codes[i].src1 == codes[i - 1].des&&codes[i-1].opr!="ASSIGN")
			{
				codes[i - 1].des = codes[i].des;
				for (int j = i; j < code_index; j++)
					codes[j] = codes[j + 1];
				code_index--;
			}
			else
				i++;
		}
		else
			i++;
	}
}
