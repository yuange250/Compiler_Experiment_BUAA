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
		for (; id_table[j].obj != "function"&&id_table[j].obj != "procedure"&&j <= tx; j++)
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
void commen_opr()
{
	
}
void generatemips()
{
	generate_aim_code("jal","MAIN_0","","");
	excute_id_table();

	for (int i = 0; i < code_index;)
	{
		if (codes[i].opr == "PLABEL"||codes[i].opr=="FLABEL")
		{
			generate_aim_code("LABEL", codes[i].des, "", "");
			string label = codes[i].des;
			int func_posi = find_func_proc_position(label);
			int aim_code_num_temp = aim_code_num;
			int temp_variables[100] = {0};
			int call_sp_dcrease = 0;
	//		
			int j;
			for (j = func_posi+1; id_table[j].obj == "const" || id_table[j].obj == "var"; j++)
			{}
			j--;
			int fp_offsite;
			if (j == func_posi)
				fp_offsite = (4 + id_table[func_posi].lev) * 4;
			else
			{
				fp_offsite = id_table[j].adr;
				if (id_table[j].type == "array")
					fp_offsite = id_table[j].adr + id_table[j].arrayinfo->size * 4;
				else
					fp_offsite = id_table[j].adr + 4;
			}
			if (codes[i].des == "MAIN_0")
			{
				generate_aim_code("subi", "$sp", "$sp", int_to_string((fp_offsite < 500) ? 500 : fp_offsite));
			}
			generate_aim_code("addi", "$fp", "$sp", int_to_string((fp_offsite < 500) ? 500 : fp_offsite));//每进一个函数，先开辟500的空间
			generate_aim_code("sw", "$ra", "$fp", int_to_string(-12));
			//generate_aim_code("subi", "$sp", "$sp", "");
			i++;
			while (codes[i].opr != "PLABEL"&&codes[i].opr != "FLABEL"&&i<code_index)
			{
				bool imediate_num = false;
				if (codes[i].opr == "CALL")
				{
					
					int call_func_posi = find_func_proc_position(codes[i].src1);
					call_sp_dcrease -= 4;
					generate_aim_code("sw", "$sp", "$sp", int_to_string(call_sp_dcrease));//保存现场，sp,fp
					call_sp_dcrease -= 4;
					generate_aim_code("sw", "$fp", "$sp", int_to_string(call_sp_dcrease));
					call_sp_dcrease -= 4;
		//			generate_aim_code("sw", "$ra", "$sp", int_to_string(call_sp_dcrease));
					for (int k = 0; k < (id_table[func_posi].lev-1)&&k<(id_table[func_posi].lev-1); k++)
					{
						call_sp_dcrease -= 4;
						generate_aim_code("lw", "$t0", "$fp", int_to_string(call_sp_dcrease));
						generate_aim_code("sw", "$t0", "$sp", int_to_string(call_sp_dcrease));
					}
					if (id_table[call_func_posi].lev - 1==id_table[func_posi].lev)
					{
						call_sp_dcrease -= 4;
						generate_aim_code("sw", "$fp", "$sp", int_to_string(call_sp_dcrease));
					}
				}
				else if (codes[i].opr == "PUSHA")
				{
					if (codes[i].src1.at(0) != '%')
					{

						int var_posi = get_the_variable(func_posi, codes[i].src1);//查找到变量在符号表中的位置
						if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
						{
							generate_aim_code("addi", "$t0", "$fp", int_to_string(-id_table[var_posi].adr));
							generate_aim_code("addi","$t2","$t0",codes[i].src2);
						}
						else
						{
							generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
							generate_aim_code("addi", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
							generate_aim_code("addi", "$t2", "$t0", codes[i].src2);
						}
					}
					call_sp_dcrease -= 4;
					generate_aim_code("sw", "$t2", "$sp", int_to_string(call_sp_dcrease));
				}
				else if (codes[i].opr == "READ")
				{
					if (codes[i].src2 == "integersym")
						generate_aim_code("li","$v0","5","");
						
					else if (codes[i].src2 == "charsym")
						generate_aim_code("li", "$v0", "12", "");
					generate_aim_code("syscall", "", "", "");
					
					int var_posi = get_the_variable(func_posi, codes[i].des);//查找到变量在符号表中的位置
					if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
					{
						generate_aim_code("addi", "$t0", "$fp", int_to_string(-id_table[var_posi].adr));
					}
					else
					{
						generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
						generate_aim_code("addi", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
					}
					generate_aim_code("sw","$v0","$t0","0");
				}
				else if (codes[i].opr == "WRITE")
				{
					if (codes[i].src2 == "integersym")
						generate_aim_code("li", "$v0", "1", "");

					else if (codes[i].src2 == "charsym")
						generate_aim_code("li", "$v0", "11", "");
					

					if (codes[i].des!=""&&(codes[i].des.at(0) != '%' && (codes[i].des.at(0) < '0' || codes[i].des.at(0) > '9')))//Store/load的des，我现在只需要将它的地址放在哪儿就好
					{
						int var_posi = get_the_variable(func_posi, codes[i].des);//查找到变量在符号表中的位置

						if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
						{
							//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
							if (id_table[var_posi].addr_or_value)
							{
								generate_aim_code("lw", "$a0", "$fp", int_to_string(-id_table[var_posi].adr));
								generate_aim_code("lw", "$a0", "$a0", "0");
							}
							else
							{
								generate_aim_code("lw", "$a0", "$fp", int_to_string(-id_table[var_posi].adr));
							}
						}
						else
						{
							generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
							if (id_table[var_posi].addr_or_value)
							{
								generate_aim_code("lw", "$a0", "$t1", int_to_string(-id_table[var_posi].adr));
								generate_aim_code("lw", "$a0", "$a0", "0");
							}
							else
							{
								generate_aim_code("lw", "$a0", "$t1", int_to_string(-id_table[var_posi].adr));
							}
						}
					}
					else
					{
						string temp_num_str;
						for (int k = 1; k < codes[i].des.size(); k++)
						{
							temp_num_str.append(1, codes[i].des.at(k));
						}
						int temp_num = atoi(temp_num_str.c_str());
						if (temp_variables[temp_num] == 0)
						{
							temp_variables[temp_num] = fp_offsite;
							fp_offsite += 4;
							if (fp_offsite >= 500)
								generate_aim_code("subi", "$sp", "$sp", "4");
						}
						generate_aim_code("lw", "$a0", "$fp", int_to_string(-temp_variables[temp_num]));
					}
					generate_aim_code("syscall", "", "", "");
				}
				else if (codes[i].opr=="STORE"||codes[i].opr=="LOAD")
				{
					int var_posi = get_the_variable(func_posi, codes[i].src1);//查找到变量在符号表中的位置
					if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
					{
						generate_aim_code("addi", "$t0", "$fp", int_to_string(-id_table[var_posi].adr));
					}
					else
					{
						generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
						generate_aim_code("addi", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
					}
					//数组偏移。。。。。。。。
					if (codes[i].src2.at(0) != '%' && (codes[i].src2.at(0) < '0' || codes[i].src2.at(0) > '9'))
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
							generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
							generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
							if (id_table[var_posi].addr_or_value)
								generate_aim_code("lw", "$t1", "$t1", "0");
						}
					}
					else if (codes[i].src2.at(0) >= '0'&&codes[i].src2.at(0) <= '9')
					{
						generate_aim_code("li", "$t1", codes[i].src2,"");
					}
					else if (codes[i].src2.at(0) == '%')
					{
						string temp_num_str;
						for (int k = 1; k < codes[i].src2.size(); k++)
						{
							temp_num_str.append(1, codes[i].src2.at(k));
						}
						int temp_num = atoi(temp_num_str.c_str());
						if (temp_variables[temp_num] == 0)
						{
							temp_variables[temp_num] = fp_offsite;
							fp_offsite += 4;
							if (fp_offsite >= 500)
								generate_aim_code("subi", "$sp", "$sp", "4");
						}
						generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
					}
				    generate_aim_code("mulo","$t1","$t1","-4");
//						generate_aim_code("addi", "$t2", "$t0", codes[i].src2);

				    generate_aim_code("add", "$t2", "$t0", "$t1");



					if (codes[i].des.at(0) != '%' && (codes[i].des.at(0) < '0' || codes[i].des.at(0) > '9'))//Store/load的des，我现在只需要将它的地址放在哪儿就好
					{
						int var_posi = get_the_variable(func_posi, codes[i].des);//查找到变量在符号表中的位置

						if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
						{
							//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
							if (id_table[var_posi].addr_or_value)
							{
								generate_aim_code("addi", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
								generate_aim_code("lw", "$t1", "$t1", "0");
							}
							else
							{
								generate_aim_code("addi", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
							}
						}
						else
						{
							generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
							if (id_table[var_posi].addr_or_value)
							{
								generate_aim_code("addi", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
								generate_aim_code("lw", "$t1", "$t1", "0");
							}
							else
							{
								generate_aim_code("addi", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
							}
						}
					}
					else
					{
						string temp_num_str;
						for (int k = 1; k < codes[i].des.size(); k++)
						{
							temp_num_str.append(1, codes[i].des.at(k));
						}
						int temp_num = atoi(temp_num_str.c_str());
						if (temp_variables[temp_num] == 0)
						{
							temp_variables[temp_num] = fp_offsite;
							fp_offsite += 4;
							if (fp_offsite >= 500)
								generate_aim_code("subi", "$sp", "$sp", "4");
						}
						generate_aim_code("addi", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
					}
					if (codes[i].opr == "STORE")
					{
						
						generate_aim_code("lw","$t0","$t1","0");
						generate_aim_code("sw","$t0","$t2","0");
					}
					else
					{
						generate_aim_code("lw","$t0","$t2","0");
						generate_aim_code("sw", "$t0", "$t1", "0");
					}
				}
				else
				{
					if (codes[i].src1 != "")
					{
						if (codes[i].src1.at(0) != '%'&&codes[i].src1.at(0) != '@' && (codes[i].src1.at(0) < '0' || codes[i].src1.at(0) > '9'))
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
								generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
								generate_aim_code("lw", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
								if (id_table[var_posi].addr_or_value)
									generate_aim_code("lw", "$t0", "$t0", "0");
							}
						}
						else if (codes[i].src1.at(0) == '@')
						{
							generate_aim_code("lw", "$t0", "$sp", "0");
						}
						else if (codes[i].src1.at(0) >= '0'&&codes[i].src1.at(0) <= '9')
						{
							generate_aim_code("li", "$t0", codes[i].src1,"");
						}
						else if (codes[i].src1.at(0)=='%')
						{
							string temp_num_str;
							for (int k = 1; k < codes[i].src1.size(); k++)
							{
								temp_num_str.append(1, codes[i].src1.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								if (fp_offsite >= 500)
									generate_aim_code("subi", "$sp", "$sp", "4");
							}
							generate_aim_code("lw", "$t0", "$fp", int_to_string(-temp_variables[temp_num]));
						}
					}
					if (codes[i].src2 != "")
					{
						//很笨的一个方法，两个参数各自分析一下，代码重复了其实
						if (codes[i].src2.at(0) != '%'&&(codes[i].src2.at(0) < '0' || codes[i].src2.at(0) > '9'))
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
								generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
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
						else if (codes[i].src2.at(0) == '%')
						{
							string temp_num_str;
							for (int k = 1; k < codes[i].src2.size(); k++)
							{
								temp_num_str.append(1, codes[i].src2.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								if (fp_offsite>=500)
									generate_aim_code("subi", "$sp", "$sp", "4");
							}
							generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
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
							generate_aim_code("sub", "$t2", "$t0", "$t1");
					}
					else if (codes[i].opr == "ASSIGN")
					{
						generate_aim_code("add", "$t2", "$zero", "$t0");
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
						generate_aim_code("LABEL", codes[i].des,"","");
					}
					else if (codes[i].opr == "JMP")
					{
						if (codes[i].des.at(0) != '_')
						{
							int call_func_posi = find_func_proc_position(label);
							int aim_code_num_temp = aim_code_num;
							//		
							int l;
							for (l = call_func_posi; id_table[l].obj == "const" || id_table[l].obj == "var"; l++)
							{
							}
							l--;
							call_sp_dcrease=id_table[l].adr;
							if (id_table[j].type == "array")
								call_sp_dcrease = id_table[j].adr + id_table[j].arrayinfo->size * 4;
							else
								call_sp_dcrease = id_table[j].adr + 4;
							call_sp_dcrease = (call_sp_dcrease < 500) ? 500 : call_sp_dcrease;
							generate_aim_code("subi","$sp","$sp",int_to_string(call_sp_dcrease));
							call_sp_dcrease = 0;
						}
						generate_aim_code("jal", codes[i].des,"","");
					}		
					else if (codes[i].opr == "PUSH")
					{
						call_sp_dcrease -= 4;
						generate_aim_code("sw", "$t0", "$sp", int_to_string(call_sp_dcrease));
					}
					else if (codes[i].opr == "RETURN")
					{
						generate_aim_code("lw", "$sp", "$fp", int_to_string(-4));
						generate_aim_code("lw","$fp","$fp",int_to_string(-8));
						generate_aim_code("lw","$ra","$sp",int_to_string(-12));
						generate_aim_code("jr","$ra","","");
					}
					if ((codes[i].opr == "ADD" || codes[i].opr == "SUB" || codes[i].opr == "MUL" || codes[i].opr == "DIV" || codes[i].opr == "OPP"||codes[i].opr=="ASSIGN")&&codes[i].des!="")
					{
						if (codes[i].des.at(0) != '%'&&(codes[i].des.at(0) < '0'||codes[i].des.at(0) > '9'))
						{
							int var_posi;
							if (id_table[func_posi].name == codes[i].des)
							{
								var_posi = func_posi;
							}
							else
								var_posi = get_the_variable(func_posi, codes[i].des);//查找到变量在符号表中的位置
							if (id_table[var_posi].obj == "function")
							{
								generate_aim_code("sw", "$t2", "$fp", "0");
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
		//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
								if (id_table[var_posi].addr_or_value)
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
									generate_aim_code("sw","$t2","$t1","0");
								}
								else
								{
									generate_aim_code("sw", "$t2", "$fp", int_to_string(-id_table[var_posi].adr));
								}
							}
							else
							{
								generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));		
								if (id_table[var_posi].addr_or_value)
								{
									generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
									generate_aim_code("sw", "$t2", "$t1", "0");
								}
								else
								{
									generate_aim_code("sw", "$t2", "$t1", int_to_string(-id_table[var_posi].adr));
								}
							}
						}
						else
						{
							string temp_num_str;
							for (int k = 1; k < codes[i].des.size(); k++)
							{
								temp_num_str.append(1, codes[i].des.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								if (fp_offsite >= 500)
									generate_aim_code("subi", "$sp", "$sp", "4");
							}
							generate_aim_code("sw", "$t2", "$fp", int_to_string(-temp_variables[temp_num]));
						}
					}
				}
				i++;
			}
			
		}
	}
}
void list_mips_code()
{
	ofstream fout("aim_code.asm");
	for (int i = 0; i < aim_code_num; i++)
	{
		
		if (aim_codes[i].opr == "addi" || aim_codes[i].opr == "subi" || aim_codes[i].opr == "mul" || aim_codes[i].opr == "mulo" || aim_codes[i].opr == "div" || aim_codes[i].opr == "add" || aim_codes[i].opr == "sub" )
		{
			fout << aim_codes[i].opr << " " << aim_codes[i].des << "," << aim_codes[i].src1 << "," << aim_codes[i].src2<<endl;
		}
		else if (aim_codes[i].opr == "bgt" || aim_codes[i].opr == "bge" || aim_codes[i].opr == "blt" || aim_codes[i].opr == "ble" || aim_codes[i].opr == "beq" || aim_codes[i].opr == "bne")
		{
			fout << aim_codes[i].opr << " " << aim_codes[i].des<< "," << aim_codes[i].src1 << "," << aim_codes[i].src2 << endl;
		}
		else if (aim_codes[i].opr == "lw" || aim_codes[i].opr == "sw")
		{
			fout << aim_codes[i].opr << " " << aim_codes[i].des << "," << aim_codes[i].src2 << "(" << aim_codes[i].src1<<")"<<endl;
		}
		else if (aim_codes[i].opr == "li")
		{
			fout << "li" << " " << aim_codes[i].des << "," << aim_codes[i].src1<<endl;
		}
		else if (aim_codes[i].opr == "LABEL")
		{
			fout << aim_codes[i].des << ":" << endl;
		}
		else if (aim_codes[i].opr == "jr")
		{
			fout << "jr"<< " " << aim_codes[i].des << endl;
		}
		else if (aim_codes[i].opr == "jal")
		{
			fout << "jal" << " " << aim_codes[i].des << endl;
		}
		else if (aim_codes[i].opr == "syscall")
		{
			fout << "syscall" << endl;
		}
		else
			fout << "这尼玛什么情况！" << endl;
	}
}
