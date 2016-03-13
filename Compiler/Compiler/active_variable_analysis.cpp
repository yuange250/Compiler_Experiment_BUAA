#include "stdafx.h"
#include "globals.h"
bool if_var_in_array(string str_var,string var_array[],int length)
{
	for (int i = 0; i < length; i++)
	{
		if (str_var == var_array[i])
			return true;
	}
	return false;
}
void mark_def_and_use()
{
	for (int i = 0; i < fb_nums; i++)
	{
		for (int j = 0; j < fbs[i].sb_nums; j++)
		{
			for (int k = 0; k < fbs[i].sbs[j].new_codes_num; k++)
			{
				char c_temp;
				
				if (fbs[i].sbs[j].new_codes[k].src1 != "")//一般是属于use集
				{
					c_temp = fbs[i].sbs[j].new_codes[k].src1.at(0);
					if ((c_temp >= 'A'&&c_temp <= 'Z') || (c_temp >= 'a'&&c_temp <= 'z'))
					{
						string oprs[] = { "ADD", "SUB", "MUL", "DIV", "OPP", "ASSIGN", "JGR", "JGE","JLS","JLE","JEQ","JNE","PUSHA","PUSH" };
						if (if_var_in_array(fbs[i].sbs[j].new_codes[k].opr, oprs,14))
						{
							if (!(if_var_in_array(fbs[i].sbs[j].new_codes[k].src1, fbs[i].sbs[j].def, fbs[i].sbs[j].def_num)
								|| if_var_in_array(fbs[i].sbs[j].new_codes[k].src1, fbs[i].sbs[j].use, fbs[i].sbs[j].use_num)))
							{
								fbs[i].sbs[j].use[fbs[i].sbs[j].use_num++] = fbs[i].sbs[j].new_codes[k].src1;
							}
						}
					}
				}

				if (fbs[i].sbs[j].new_codes[k].src2 != "")//一般是属于use集
				{
					c_temp = fbs[i].sbs[j].new_codes[k].src2.at(0);
					if ((c_temp >= 'A'&&c_temp <= 'Z') || (c_temp >= 'a'&&c_temp <= 'z'))
					{
						string oprs[] = { "ADD", "SUB", "MUL", "DIV", "JGR", "JGE", "JLS", "JLE", "JEQ", "JNE", "PUSHA","STORE","LOAD" };
						if (if_var_in_array(fbs[i].sbs[j].new_codes[k].opr, oprs, 13))
						{
							if (!(if_var_in_array(fbs[i].sbs[j].new_codes[k].src2, fbs[i].sbs[j].def, fbs[i].sbs[j].def_num)
								|| if_var_in_array(fbs[i].sbs[j].new_codes[k].src2, fbs[i].sbs[j].use, fbs[i].sbs[j].use_num)))
							{
								fbs[i].sbs[j].use[fbs[i].sbs[j].use_num++] = fbs[i].sbs[j].new_codes[k].src2;
							}
						}
					}
				}

				if (fbs[i].sbs[j].new_codes[k].des != "")//一般是属于def集
				{
					c_temp = fbs[i].sbs[j].new_codes[k].des.at(0);
					if ((c_temp >= 'A'&&c_temp <= 'Z') || (c_temp >= 'a'&&c_temp <= 'z'))
					{
						string oprs[] = { "ADD", "SUB", "MUL", "DIV", "OPP", "ASSIGN", "LOAD", "READ" };
						if (fbs[i].sbs[j].new_codes[k].opr == "WRITE" || fbs[i].sbs[j].new_codes[k].opr == "STORE")
						{
							if (!(if_var_in_array(fbs[i].sbs[j].new_codes[k].des, fbs[i].sbs[j].def, fbs[i].sbs[j].def_num)
								|| if_var_in_array(fbs[i].sbs[j].new_codes[k].des, fbs[i].sbs[j].use, fbs[i].sbs[j].use_num)))
							{
								fbs[i].sbs[j].use[fbs[i].sbs[j].use_num++] = fbs[i].sbs[j].new_codes[k].des;
							}
						}
						else if (if_var_in_array(fbs[i].sbs[j].new_codes[k].opr, oprs, 8))
						{
							if (!(if_var_in_array(fbs[i].sbs[j].new_codes[k].des, fbs[i].sbs[j].def, fbs[i].sbs[j].def_num)
								|| if_var_in_array(fbs[i].sbs[j].new_codes[k].des, fbs[i].sbs[j].use, fbs[i].sbs[j].use_num)))
							{
								fbs[i].sbs[j].def[fbs[i].sbs[j].def_num++] = fbs[i].sbs[j].new_codes[k].des;
							}
						}
					}
				}
			}
		}
	}
}

void data_analysis()
{
	
	for (int i = 0; i < fb_nums; i++)
	{
		int continue_flag = true;
		while (continue_flag)
		{
			continue_flag = false;
			for (int j = fbs[i].sb_nums-1; j >= 0; j--)
			{
				string in_temp[40];
				int in_temp_num = 0;
				fbs[i].sbs[j].out_num = 0;
				for (int k = fbs[i].sbs[j].bb_num - 1; k >= 0; k--)
				{
					
					for (int l = fbs[i].sbs[fbs[i].sbs[j].backward_blocks_num[k]].in_num-1; l >= 0; l--)
					{
						if (!if_var_in_array(fbs[i].sbs[fbs[i].sbs[j].backward_blocks_num[k]].in[l], fbs[i].sbs[j].out, fbs[i].sbs[j].out_num))
						{
							fbs[i].sbs[j].out[fbs[i].sbs[j].out_num++] = fbs[i].sbs[fbs[i].sbs[j].backward_blocks_num[k]].in[l];
						}
						if (!if_var_in_array(fbs[i].sbs[fbs[i].sbs[j].backward_blocks_num[k]].in[l], fbs[i].sbs[j].def, fbs[i].sbs[j].def_num)
							&& !if_var_in_array(fbs[i].sbs[fbs[i].sbs[j].backward_blocks_num[k]].in[l], in_temp, in_temp_num))
						{
							in_temp[in_temp_num++] = fbs[i].sbs[fbs[i].sbs[j].backward_blocks_num[k]].in[l];
							
						}
					}
					
				}
				
				for (int m = fbs[i].sbs[j].use_num-1; m >= 0; m--)
				{
					if (!if_var_in_array(fbs[i].sbs[j].use[m], in_temp, in_temp_num))
						in_temp[in_temp_num++] = fbs[i].sbs[j].use[m];
				}
				for (int m = in_temp_num - 1; m >= 0; m--)
				{
					if (!if_var_in_array(in_temp[m], fbs[i].sbs[j].in, fbs[i].sbs[j].in_num))
					{
						fbs[i].sbs[j].in[fbs[i].sbs[j].in_num++] = in_temp[m];
						continue_flag = true;
					}
				}
			}
		}
	}
}
void global_rigister_spare()
{
	int conflict[30][30] = { 0 };
	int conflict2[30][30] = { 0 };
	for (int i = 0; i < fb_nums; i++)
	{
		memset(conflict,0,900);
		memset(conflict2, 0, 900);
		string var_stack[30];
		int var_stack_num = 0;
		map<string, int> var_number;
		string vars[30];
		int var_nums = 0;
		for (int j = 0; j < fbs[i].sb_nums; j++)
		{
			for (int k = 0; k < fbs[i].sbs[j].in_num; k++)
			{
				map<string, int>::iterator it2 = var_number.find(fbs[i].sbs[j].in[k]);
				if (it2 == var_number.end())
				{
					vars[var_nums] = fbs[i].sbs[j].in[k];
					var_number[fbs[i].sbs[j].in[k]] = var_nums++;
				}
				for (int l = k + 1; l < fbs[i].sbs[j].in_num; l++)
				{
					it2 = var_number.find(fbs[i].sbs[j].in[l]);
					if (it2 == var_number.end())
					{
						vars[var_nums] = fbs[i].sbs[j].in[l];
						var_number[fbs[i].sbs[j].in[l]] = var_nums++;
					}
					conflict[var_number[fbs[i].sbs[j].in[k]]][var_number[fbs[i].sbs[j].in[l]]] = 1;
					conflict[var_number[fbs[i].sbs[j].in[l]]][var_number[fbs[i].sbs[j].in[k]]] = 1;
					conflict2[var_number[fbs[i].sbs[j].in[k]]][var_number[fbs[i].sbs[j].in[l]]] = 1;
					conflict2[var_number[fbs[i].sbs[j].in[l]]][var_number[fbs[i].sbs[j].in[k]]] = 1;
				}
			}
		}
		if (var_nums < global_rigister_max)
		{
			for (int j = 0; j < var_nums; j++)
			{
				fbs[i].global_rigisters[vars[j]] = j;
				fbs[i].registers[j] = vars[j];
			}
		}
		else{
			int pick_out_num = 0;
			while (pick_out_num != var_nums)
			{
				bool cut_off_flag = true;
				for (int j = 0; j < var_nums; j++)
				{
					if (if_var_in_array(vars[j], var_stack, var_stack_num))
					{
						continue;
					}
					int connect_num = 0;
					for (int k = 0; k < var_nums; k++)
					{
						if (conflict[var_number[vars[j]]][var_number[vars[k]]] == 1)
							connect_num++;
					}
					if (connect_num < global_rigister_max)
					{
						var_stack[var_stack_num++] = vars[j];
						pick_out_num++;
						cut_off_flag = false;
						for (int k = 0; k < var_nums; k++)
						{
							conflict[var_number[vars[k]]][var_number[vars[j]]] = 0;
							conflict[var_number[vars[j]]][var_number[vars[k]]] = 0;
						}
						break;
					}
					/*map<string, int>::iterator itr = var_number.find(vars[j]);
					if (itr == var_number.end())
					{

					}*/
				}
				if (cut_off_flag)
				{
					for (int j = 0; j < var_nums; j++)
					{
						if (if_var_in_array(vars[j], var_stack, var_stack_num))
						{
							continue;
						}
						fbs[i].global_rigisters[vars[j]] = -1;
						pick_out_num++;
						for (int k = 0; k < var_nums; k++)
						{
							conflict[var_number[vars[k]]][var_number[vars[j]]] = 0;
							conflict[var_number[vars[j]]][var_number[vars[k]]] = 0;
						}
					}
				}
			}
        //spare the registers
			for (int j = var_stack_num - 1; j >= 0; j--)
			{
				for (int k = 0; k < global_rigister_max; k++)
				{
					bool if_register_ok = true;
					for (int l = j + 1; l < var_stack_num; l++)
					{
						if (conflict2[var_number[var_stack[l]]][var_number[var_stack[j]]])
						{
							if (fbs[i].global_rigisters[var_stack[l]] == k)
							{
								if_register_ok = false;
							}
						}
					}
					if (if_register_ok&&fbs[i].registers[k]=="")
					{
						fbs[i].global_rigisters[var_stack[j]] == k;
						fbs[i].registers[k] = var_stack[j];
						break;
					}
				}
			}

		}
	}
}
void active_var_analysis()
{
	mark_def_and_use();
	data_analysis();
	global_rigister_spare();
	ofstream fout("optimize_debug.txt");

	for (int i = 0; i < fb_nums; i++)
	{
		fout << "FUNCTION:"<<i << endl;
		fout << "-------------------------------------------" << endl;
		for (int j = 0; j < fbs[i].sb_nums; j++)
		{
			fout << "STANDARD BLOCK: " <<j << endl;
			fout << "***********************************" << endl;
			fout << "new codes:" << endl;
			for (int k = 0; k < fbs[i].sbs[j].new_codes_num; k++)
			{
				fout << fbs[i].sbs[j].new_codes[k].opr << " " << fbs[i].sbs[j].new_codes[k].src1 << " " << fbs[i].sbs[j].new_codes[k].src2 << " " << fbs[i].sbs[j].new_codes[k].des << endl;
			}
			fout << "def:";
			for (int k = 0; k < fbs[i].sbs[j].def_num; k++)
			{
				fout << fbs[i].sbs[j].def[k] << " ";
			}
			fout << endl;
			fout << "use:";
			for (int k = 0; k < fbs[i].sbs[j].use_num; k++)
			{
				fout << fbs[i].sbs[j].use[k] << " ";
			}
			fout << endl;
			fout << "in:";
			for (int k = 0; k < fbs[i].sbs[j].in_num; k++)
			{
				fout << fbs[i].sbs[j].in[k] << " ";
			}
			fout << endl;
			fout << "out:" ;
			for (int k = 0; k < fbs[i].sbs[j].out_num; k++)
			{
				fout << fbs[i].sbs[j].out[k]<<" " ;
			}
			fout << endl;
			fout << "forward:";
			for (int k = 0; k < fbs[i].sbs[j].fb_num; k++)
			{
				fout << fbs[i].sbs[j].forward_blocks_num[k] << " ";
			}
			fout << endl;
			fout << "backward:";
			for (int k = 0; k < fbs[i].sbs[j].bb_num; k++)
			{
				fout << fbs[i].sbs[j].backward_blocks_num[k] << " ";
			}
			fout << endl;
			fout << "***********************************" << endl;
		}
		fout << "global registers" << endl;
		map<string, int>::iterator itr = fbs[i].global_rigisters.begin();
		while (itr != fbs[i].global_rigisters.end())
		{
			fout << itr->first << ":  " << itr->second << endl;
			itr++;
		}
		fout << "-------------------------------------------" << endl;
	}
}
