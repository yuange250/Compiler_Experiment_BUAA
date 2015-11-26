#include "stdafx.h"
#include "globals.h"
ofstream fout("test.asm");
struct code aim_codes[2000];
int aim_code_num = 0;
int find_func_proc_position(string label);
int get_the_variable(int posi, string name);
string int_to_string(int num);
void excute_id_table()
{
	for (int i = 1; i <= tx; )
	{
	//	int func_or_proc = (id_table[i].obj == "function") ? 1 : 0;
		int j = i + 1;
		int current_addr = (4 + id_table[j].lev-1) * 4;
		for (int k = 0; id_table[j].obj != "function"&&id_table[j].obj != "procedure"&&j <= tx; k++,j++)
		{
			id_table[j].adr = current_addr;
			if (id_table[j].type == "charsym" || id_table[j].type == "integersym")
				current_addr += 4;
			else if (id_table[j].type == "array")
			{
				current_addr += id_table[j].arrayinfo->size * 4;
			}
		}
		i = j;
	}
}
void generate_aim_code(string opr, string des, string src1, string src2)
{
	aim_codes[aim_code_num].opr = opr;
	aim_codes[aim_code_num].des = des;
	aim_codes[aim_code_num].src1 = src1;
	aim_codes[aim_code_num++].src2 = src2;
}
void generatemips()
{
	generate_aim_code("jal","MAIN_0","","");

	for (int i = 0; i < code_index; i++)
	{
		if (codes[i].opr == "PLABEL")
		{
			string label = codes[i].des;
			int func_posi = find_func_proc_position(label);
			int aim_code_num_temp = aim_code_num;
			int temp_variables[100] = {0};
	//		
			int j;
			for (j = i; id_table[j].obj == "const" || id_table[j].obj == "var"; j++)
			{}
			j--;
			int fp_offsite;
			if (id_table[j].type == "array")
				fp_offsite = id_table[j].adr + id_table[j].arrayinfo->size * 4;
			else
				fp_offsite = id_table[j].adr + 4;
			generate_aim_code("addi", "$fp", "$sp", int_to_string(fp_offsite));
			//generate_aim_code("subi", "$sp", "$sp", "");
			while (codes[i].opr != "PLABEL"&&codes[i].opr != "FLABEL")
			{
				bool imediate_num = false;
				if (codes[i].opr == "CALL")
				{
					int call_sp_dcrease = 0;
					int call_func_posi = find_func_proc_position(codes[i].src1);
					call_sp_dcrease -= 4;
					generate_aim_code("sw", "$sp", "$sp", int_to_string(call_sp_dcrease));//保存现场，sp,fp
					call_sp_dcrease -= 4;
					generate_aim_code("sw", "$fp", "$sp", int_to_string(call_sp_dcrease));
					call_sp_dcrease -= 4;
					generate_aim_code("sw", "$ra", "$sp", int_to_string(call_sp_dcrease));
					for (int k = 0; k < (id_table[func_posi].lev-1)&&k<(id_table[func_posi].lev-1); k++)
					{
						generate_aim_code("lw", "$t0", "$fp", int_to_string(call_sp_dcrease));
						generate_aim_code("sw", "$t0", "$sp", int_to_string(call_sp_dcrease));
						call_sp_dcrease -= 4;
					}
					if (id_table[call_func_posi].lev - 1==id_table[call_func_posi].lev)
					{
						generate_aim_code("sw", "$fp", "$sp", int_to_string(call_sp_dcrease));
						call_sp_dcrease -= 4;
					}

				}
				else
				{
					if (codes[i].src1 != "")
					{
						if (codes[i].src1.at(0) != '$'&&codes[i].src1.at(0) < '0'&&codes[i].src1.at(0) > '9')
						{
							int var_posi = get_the_variable(func_posi, codes[i].src1);//查找到变量在符号表中的位置
							if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								generate_aim_code("lw", "$t0", "$fp", int_to_string(-id_table[var_posi].adr));
								if (id_table[var_posi].addr_or_value)
									generate_aim_code("lw", "$t0", "$t0", "0");
							}
							else
							{
								generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[j].lev - 1) * 4));
								generate_aim_code("lw", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
								if (id_table[var_posi].addr_or_value)
									generate_aim_code("lw", "$t0", "$t0", "0");
							}
						}
						else if (codes[i].src1.at(0) >= '0'&&codes[i].src1.at(0) <= '9')
						{
							generate_aim_code("li", "$t0", "", codes[i].src1);
						}
						else
						{
							string temp_num_str;
							for (int k = 1; k < codes[i].src1.at(k); k++)
							{
								temp_num_str.append(1, codes[i].src1.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								generate_aim_code("subi", "$sp", "$sp", "4");
							}
							generate_aim_code("lw", "$t0", "$fp", int_to_string(-id_table[temp_num].adr));
						}
					}
					if (codes[i].src2 != "")
					{
						//很笨的一个方法，两个参数各自分析一下，代码重复了其实
						if (codes[i].src2.at(0) != '$'&&codes[i].src2.at(0) < '0'&&codes[i].src2.at(0) > '9')
						{
							int var_posi = get_the_variable(func_posi, codes[i].src2);//查找到变量在符号表中的位置
							if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
								if (id_table[var_posi].addr_or_value)
									generate_aim_code("lw", "$t1", "$t1", "0");
							}
							else
							{
								generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[j].lev - 1) * 4));
								generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
								if (id_table[var_posi].addr_or_value)
									generate_aim_code("lw", "$t1", "$t1", "0");
							}
						}
						else if (codes[i].src2.at(0) >= '0'&&codes[i].src2.at(0) <= '9')
						{
							imediate_num = true;
							//			generate_aim_code("li", "$t1", "", codes[i].src2);
						}
						else
						{
							string temp_num_str;
							for (int k = 1; k < codes[i].src2.at(k); k++)
							{
								temp_num_str.append(1, codes[i].src2.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								generate_aim_code("subi", "$sp", "$sp", "4");
							}
							generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[temp_num].adr));
						}
					}
					if (codes[i].opr == "ADD")
					{
						if (imediate_num)
							generate_aim_code("addi", "$t2", "$t0", codes[i].src2);
						else
							generate_aim_code("add", "$t2", "$t0", "$t1");

					}
					else if (codes[i].opr == "SUB")
					{
						if (imediate_num)
							generate_aim_code("subi", "$t2", "$t0", codes[i].src2);
						else
							generate_aim_code("sub", "$t2", "$t0", "$t1");
					}
					else if (codes[i].opr == "MUL")
					{
						if (imediate_num)
							generate_aim_code("mul", "$t2", "$t0", codes[i].src2);
						else
							generate_aim_code("mulo", "$t2", "$t0", "$t1");
					}
					else if (codes[i].opr == "DIV")
					{
						if (imediate_num)
							generate_aim_code("div", "$t2", "$t0", codes[i].src2);
						else
							generate_aim_code("div", "$t2", "$t0", "$t1");
					}
					else if (codes[i].opr == "OPP")
					{
						if (imediate_num)
							generate_aim_code("subi", "$t2", "$t0", codes[i].src2);
						else
							generate_aim_code("subi", "$t2", "$t0", "$t1");
					}
					else if (codes[i].opr == "JGR")
					{
						if (imediate_num)
							generate_aim_code("bgt", "$t0", codes[i].src2, codes[i].des);
						else
							generate_aim_code("bgt", "$t0", "$t1", codes[i].des);
					}
					else if (codes[i].opr == "JGE")
					{
						if (imediate_num)
							generate_aim_code("bge", "$t0", codes[i].src2, codes[i].des);
						else
							generate_aim_code("bge", "$t0", "$t1", codes[i].des);
					}
					else if (codes[i].opr == "JLS")
					{
						if (imediate_num)
							generate_aim_code("blt", "$t0", codes[i].src2, codes[i].des);
						else
							generate_aim_code("blt", "$t0", "$t1", codes[i].des);
					}
					else if (codes[i].opr == "JLE")
					{
						if (imediate_num)
							generate_aim_code("ble", "$t0", codes[i].src2, codes[i].des);
						else
							generate_aim_code("ble", "$t0", "$t1", codes[i].des);
					}
					else if (codes[i].opr == "JEQ")
					{
						if (imediate_num)
							generate_aim_code("beq", "$t0", codes[i].src2, codes[i].des);
						else
							generate_aim_code("beq", "$t0", "$t1", codes[i].des);
					}
					else if (codes[i].opr == "JNE")
					{
						if (imediate_num)
							generate_aim_code("bne", "$t0", codes[i].src2, codes[i].des);
						else
							generate_aim_code("bne", "$t0", "$t1", codes[i].des);
					}
					else if (codes[i].opr == "LABEL")
					{
						generate_aim_code("LABEL", "", "", codes[i].des);
					}
					else if (codes[i].opr == "JMP")
					{
						generate_aim_code("jal", "", "", codes[i].des);
					}
					else if (codes[i].opr == "ASSIGN")
					{
						generate_aim_code("add", "$t2", "$zero", "$t0");
					}
				}
			}
			
		}
	}
}
