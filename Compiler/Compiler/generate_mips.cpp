#include "stdafx.h"
#include "globals.h"
//ofstream fout("test.asm");
struct code aim_codes[2000];
int aim_code_num = 0;
int find_func_proc_position(string label);
int get_the_variable(int posi, string name);
string int_to_string(int num);
string generate_string_Label(int string_num_);
void excute_id_table()
{
	for (int i = 1; i <= tx; )
	{
	//	int func_or_proc = (id_table[i].obj == "function") ? 1 : 0;
		int j = i + 1;
		int current_addr = (4 + id_table[j].lev-1) * 4;
		for (; id_table[j].obj != "function"&&id_table[j].obj != "procedure"&&j <= tx; j++)
		{
			if (id_table[j].obj == "const")
				continue;
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
int if_in_map_or_not(string var_,map<string,int> in_map)
{
	map<string, int>::iterator itr = in_map.find(var_);
	if (itr == in_map.end())
	{
		return -1;
	}
	else
		return in_map[var_];
}
string if_register_occupied(int register_num,map<string,int> in_map)
{
	map<string, int>::iterator itr = in_map.begin();
	while (itr != in_map.end())
	{
		if (itr->second == register_num)
		{
			return itr->first;
		}
		itr++;
	}
	return "";
}
void save_occupied(int func_posi, int register_num, string occupied_var)
{
	int occupied_var_posi = get_the_variable(func_posi, occupied_var);
	if (id_table[occupied_var_posi].lev == (id_table[func_posi].lev + 1))
	{
		//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
		if (id_table[occupied_var_posi].addr_or_value)
		{
			generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[occupied_var_posi].adr));
			generate_aim_code("sw", "$s" + int_to_string(register_num), "$t1", "0");
		}
		else
		{
			generate_aim_code("sw", "$s" + int_to_string(register_num), "$fp", int_to_string(-id_table[occupied_var_posi].adr));
		}
	}
	else
	{
		generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[occupied_var_posi].lev - 1) * 4));
		if (id_table[occupied_var_posi].addr_or_value)
		{
			generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[occupied_var_posi].adr));
			generate_aim_code("sw", "$s" + int_to_string(register_num), "$t1", "0");
		}
		else
		{
			generate_aim_code("sw", "$s" + int_to_string(register_num), "$t1", int_to_string(-id_table[occupied_var_posi].adr));

		}
	}
}
bool temp_if_saved(int f_num,int s_num,int c_num,string temp_var)
{
	int scan_num = 0;
	for (scan_num = 0; scan_num < temp_var_max_depth&&s_num < fbs[f_num].sb_nums; s_num++)
	{
		for (; scan_num < temp_var_max_depth&&c_num < fbs[f_num].sbs[s_num].new_codes_num; c_num++)
		{
			if (fbs[f_num].sbs[s_num].new_codes[c_num].src1 == temp_var || fbs[f_num].sbs[s_num].new_codes[c_num].src2 == temp_var || (fbs[f_num].sbs[s_num].new_codes[c_num].opr == "STORE"&& fbs[f_num].sbs[s_num].new_codes[c_num].des == temp_var))
			{
				return true;
			}
			scan_num++;
		}
		c_num = 0;
	}
	return false;
}
bool if_should_save(int f_num,int s_num,int c_num,string global_var)
{
	for (int i = s_num,j=c_num-1; i >= 0; i--)
	{

		for (; j >= 0; j--)
		{
			if (fbs[f_num].sbs[i].new_codes[j].des==global_var&&fbs[f_num].sbs[i].new_codes[j].opr!="STORE")
			{
				return true;
			}
			if (fbs[f_num].sbs[j].new_codes[j].opr == "JMP"&&fbs[f_num].sbs[j].new_codes[j].des.at(0) != '_')
			{
				return false;
			}
		}
		if (i >= 1)
		{
			j = fbs[f_num].sbs[i - 1].new_codes_num - 1;
		}
	}
	return false;
}
bool if_should_load(int f_num,int s_num,int c_num,string global_var)
{
	for (int i = s_num, j = c_num + 1; i < fbs[f_num].sb_nums; i++)
	{

		for (; j < fbs[f_num].sbs[i].new_codes_num; j++)
		{
			if ((fbs[f_num].sbs[i].new_codes[j].src1 == global_var || fbs[f_num].sbs[i].new_codes[j].src2 == global_var) || (fbs[f_num].sbs[i].new_codes[j].opr != "LOAD"&& fbs[f_num].sbs[i].new_codes[j].des == global_var))
			{
				return true;
			}
			if (fbs[f_num].sbs[j].new_codes[j].opr == "JMP"&&fbs[f_num].sbs[j].new_codes[j].des.at(0) != '_')
			{
				return false;
			}
		}
		j = 0;

	}
	return false;
}
void generatemips_new()
{
	generate_aim_code("jal", "MAIN_0", "", "");
	excute_id_table();
	for (int g = 0; g < fb_nums; g++)
	{
		generate_aim_code("LABEL", fbs[g].sbs[0].new_codes[0].des, "", "");
		string label = fbs[g].sbs[0].new_codes[0].des;
		int func_posi = find_func_proc_position(label);
		int aim_code_num_temp = aim_code_num;
		int temp_variables[200] = { 0 };
		int call_sp_dcrease = 0;
		//		
		int j;
		int fp_offsite;
//		map<string, int> var_rigisters;
		map<string, int> temp_rigisters;
		string rigisters_condition[8];
//		string global_rigisters_condition[8];
		int temp_rigister = 0;
		for (j = func_posi + 1; id_table[j].obj == "const" || id_table[j].obj == "var"; j++)
		{
		}
		while (id_table[j].obj != "var"&&j != func_posi)
			j--;

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

		if (fbs[g].sbs[0].new_codes[0].des == "MAIN_0")
		{
			generate_aim_code("subi", "$sp", "$sp", int_to_string((fp_offsite < 500) ? 500 : fp_offsite));
		}

		generate_aim_code("addi", "$fp", "$sp", int_to_string((fp_offsite < 500) ? 500 : fp_offsite));//每进一个函数，先开辟500的空间
		generate_aim_code("sw", "$ra", "$fp", int_to_string(-12));
		//generate_aim_code("subi", "$sp", "$sp", "");

		for (int k = 0; k < 8; k++)
		{
			if (fbs[g].registers[k] != "")
			{
				int var_posi_temp = get_the_variable(func_posi, fbs[g].registers[k]);
				if (id_table[var_posi_temp].obj != "var")
				{
					if (id_table[var_posi_temp].obj == "const")
					{
						generate_aim_code("li", "$s" + int_to_string(k),int_to_string(id_table[var_posi_temp].value), "");
					}
					continue;
				}
				if (if_should_load(g, 0, 0, fbs[g].registers[k]))
				{
					if (id_table[var_posi_temp].lev == (id_table[func_posi].lev + 1))
					{
						//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
						if (id_table[var_posi_temp].addr_or_value)
						{
							generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi_temp].adr));
							generate_aim_code("lw", "$s" + int_to_string(k), "$t1", "0");
						}
						else
						{
							generate_aim_code("lw", "$s" + int_to_string(k), "$fp", int_to_string(-id_table[var_posi_temp].adr));
						}
					}
					else
					{
						generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi_temp].lev - 1) * 4));
						if (id_table[var_posi_temp].addr_or_value)
						{
							generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi_temp].adr));
							generate_aim_code("lw", "$s" + int_to_string(k), "$t1", "0");
						}
						else
						{
							generate_aim_code("lw", "$s" + int_to_string(k), "$t1", int_to_string(-id_table[var_posi_temp].adr));

						}
					}
				}
			}
		}

		for (int h = 0; h < fbs[g].sb_nums; h++)
		{

			for (int i = 0; i < fbs[g].sbs[h].new_codes_num; i++)
			{

				if (fbs[g].sbs[h].new_codes[i].opr == "PLABEL" || fbs[g].sbs[h].new_codes[i].opr == "FLABEL")
				{
					continue;
				}
				while (i < fbs[g].sbs[h].new_codes_num)
				{
					bool imediate_num = false;
					if (fbs[g].sbs[h].new_codes[i].opr == "CALL")
					{

						int call_func_posi = find_func_proc_position(fbs[g].sbs[h].new_codes[i].src1);
						call_sp_dcrease -= 4;
						generate_aim_code("sw", "$sp", "$sp", int_to_string(call_sp_dcrease));//保存现场，sp,fp
						call_sp_dcrease -= 4;
						generate_aim_code("sw", "$fp", "$sp", int_to_string(call_sp_dcrease));
						call_sp_dcrease -= 4;
						//			generate_aim_code("sw", "$ra", "$sp", int_to_string(call_sp_dcrease));
						for (int k = 0; k < id_table[func_posi].lev&&k < id_table[call_func_posi].lev; k++)
						{
							call_sp_dcrease -= 4;
							generate_aim_code("lw", "$t0", "$fp", int_to_string(call_sp_dcrease));
							generate_aim_code("sw", "$t0", "$sp", int_to_string(call_sp_dcrease));
						}
						if (id_table[call_func_posi].lev - 1 == id_table[func_posi].lev)
						{
							call_sp_dcrease -= 4;
							generate_aim_code("sw", "$fp", "$sp", int_to_string(call_sp_dcrease));
						}
						
					}
					else if (fbs[g].sbs[h].new_codes[i].opr == "PUSHA")
					{
						string src2_register;
						bool if_offsite_zero=false;
						if (fbs[g].sbs[h].new_codes[i].src1.at(0) != '%')
						{

							int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].src1);//查找到变量在符号表中的位置

							if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								generate_aim_code("addi", "$t0", "$fp", int_to_string(-id_table[var_posi].adr));

							}
							else
							{
								generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
								generate_aim_code("addi", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
								//		generate_aim_code("addi", "$t2", "$t0", codes[i].src2);
							}
							if (id_table[var_posi].addr_or_value)
							{
								generate_aim_code("lw", "$t0", "$t0", "0");
							}
						}
						if (fbs[g].sbs[h].new_codes[i].src2.at(0) >= '0'&&fbs[g].sbs[h].new_codes[i].src2.at(0) <= '9')
						{
							if (fbs[g].sbs[h].new_codes[i].src2 == "0")
							{
								if_offsite_zero = true;
							}
							else
							{
								generate_aim_code("li", "$t1", fbs[g].sbs[h].new_codes[i].src2, "");
								src2_register = "$t1";
							}
						}
						else  if (fbs[g].sbs[h].new_codes[i].src2.at(0) == '%')
						{
							string temp_num_str;
							for (int k = 1; k < fbs[g].sbs[h].new_codes[i].src2.size(); k++)
							{
								temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].src2.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								if (fp_offsite >= 500)
									generate_aim_code("subi", "$sp", "$sp", "4");
							}
							if (rigisters_condition[temp_rigisters[fbs[g].sbs[h].new_codes[i].src2]] == fbs[g].sbs[h].new_codes[i].src2)
							{
								src2_register = "$t" + int_to_string(temp_rigisters[fbs[g].sbs[h].new_codes[i].src2]+3);
							}
							else
							{
								src2_register = "$t1";
								generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
							}
							//generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));

						}
						else
						{
							int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].src2);//查找到变量在符号表中的位置
							bool register_flag = false;
							bool load_register_flag = false;

							int register_num = 0;
							if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
							{
								register_flag = true;
							}
							if (id_table[var_posi].obj == "const")
							{
								if (register_flag)
								{
									src2_register = "$s" + int_to_string(register_num);
									
									if (load_register_flag)
										generate_aim_code("li", src2_register, int_to_string(id_table[var_posi].value), "");
								}
								else
								{
									generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
									src2_register = "$t1";
								}
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								if (register_flag)
								{
									src2_register = "$s" + int_to_string(register_num);
		//							global_rigisters_condition[register_num] = id_table[var_posi].name;
									/*if (load_register_flag)
									{
										generate_aim_code("lw", src2_register, "$fp", int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", src2_register, src2_register, "0");
									}*/
								}
								else
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
									if (id_table[var_posi].addr_or_value)
										generate_aim_code("lw", "$t1", "$t1", "0");
									src2_register = "$t1";
								}
							}
							else
							{
								if (register_flag)
								{
									src2_register = "$s" + int_to_string(register_num);
	//								global_rigisters_condition[register_num] = id_table[var_posi].name;
								/*	if (load_register_flag)
									{
										generate_aim_code("lw", src2_register, "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
										generate_aim_code("lw", src2_register, src2_register, int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", src2_register, src2_register, "0");
									}*/
								}
								else
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
									generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
									if (id_table[var_posi].addr_or_value)
										generate_aim_code("lw", "$t1", "$t1", "0");
									src2_register = "$t1";
								}
							}
						}
						if (!if_offsite_zero)
						{
							generate_aim_code("sll", "$t1", src2_register, "2");
							generate_aim_code("sub", "$t2", "$t0", "$t1");
							call_sp_dcrease -= 4;
							generate_aim_code("sw", "$t2", "$sp", int_to_string(call_sp_dcrease));
						}
						else
						{
							call_sp_dcrease -= 4;
							generate_aim_code("sw", "$t0", "$sp", int_to_string(call_sp_dcrease));
						}
					}


					else if (fbs[g].sbs[h].new_codes[i].opr == "READ")
					{
						if (fbs[g].sbs[h].new_codes[i].src2 == "integersym")
							generate_aim_code("li", "$v0", "5", "");

						else if (fbs[g].sbs[h].new_codes[i].src2 == "charsym")
							generate_aim_code("li", "$v0", "12", "");
						generate_aim_code("syscall", "", "", "");

						int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].des);//查找到变量在符号表中的位置
						int register_num = 0;
						if ((register_num=if_in_map_or_not(id_table[var_posi].name,fbs[g].global_rigisters)) != -1)
						{
							/*if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
							{
								string occupied_var;
								if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
								{
									save_occupied(func_posi,register_num,occupied_var);
									var_rigisters[occupied_var] = -1;
									var_rigisters[id_table[var_posi].name] = register_num;
								}
							}
							global_rigisters_condition[register_num] = id_table[var_posi].name;*/
							generate_aim_code("add", "$s" + int_to_string(register_num),"$v0", "$zero");
							
						}
						else
						{
							if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								generate_aim_code("sw", "$v0", "$fp", int_to_string(-id_table[var_posi].adr));
							}
							else
							{
								generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
								generate_aim_code("sw", "$v0", "$t0", int_to_string(-id_table[var_posi].adr));
							}
						}
		//				generate_aim_code("sw", "$v0", "$t0", "0");
					}
					else if (fbs[g].sbs[h].new_codes[i].opr == "WRITE")
					{
						if (fbs[g].sbs[h].new_codes[i].src2 == "string")
						{
							generate_aim_code("la", "$a0", fbs[g].sbs[h].new_codes[i].des, "");
							generate_aim_code("li", "$v0", "4", "");
							generate_aim_code("syscall", "", "", "");
						}
						else
						{
							if (fbs[g].sbs[h].new_codes[i].src2 == "integersym")
								generate_aim_code("li", "$v0", "1", "");

							else if (fbs[g].sbs[h].new_codes[i].src2 == "charsym")
								generate_aim_code("li", "$v0", "11", "");


							if (fbs[g].sbs[h].new_codes[i].des != "" && (fbs[g].sbs[h].new_codes[i].des.at(0) != '%' && (fbs[g].sbs[h].new_codes[i].des.at(0) < '0' || fbs[g].sbs[h].new_codes[i].des.at(0) > '9')))//Store/load的des，我现在只需要将它的地址放在哪儿就好
							{
								bool register_flag = false;
								bool load_register_flag = false;
								int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].des);//查找到变量在符号表中的位置
								int register_num = 0;
								if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
								{
									/*if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
									{
										string occupied_var;
										if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
										{
											save_occupied(func_posi, register_num, occupied_var);
											var_rigisters[occupied_var] = -1;
											var_rigisters[id_table[var_posi].name] = register_num;
										}
										load_register_flag = true;
									}
									global_rigisters_condition[register_num] = id_table[var_posi].name;*/
									register_flag = true;
								}
								//		generate_aim_code("add", "$v0", "$zero", "$s" + int_to_string(register_num));

								if (id_table[var_posi].obj == "const")
								{
									if (register_flag)
									{
										/*if (load_register_flag)
											generate_aim_code("li", "$s" + int_to_string(register_num), int_to_string(id_table[var_posi].value),"");*/
										generate_aim_code("add", "$a0", "$s" + int_to_string(register_num),"$zero");
									}
									else
										generate_aim_code("li", "$a0", int_to_string(id_table[var_posi].value), "");
								}
								else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
								{
									//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
									
									if (register_flag)
									{
										/*if (load_register_flag)
										{
											if (id_table[var_posi].addr_or_value)
											{
												generate_aim_code("lw", "$s" + int_to_string(register_num), "$fp", int_to_string(-id_table[var_posi].adr));
												generate_aim_code("lw", "$s" + int_to_string(register_num), "$s" + int_to_string(register_num), "0");
											}
											else
											{
												generate_aim_code("lw", "$s" + int_to_string(register_num), "$fp", int_to_string(-id_table[var_posi].adr));
											}
										}*/
										generate_aim_code("add", "$a0", "$s" + int_to_string(register_num), "$zero");
									}
									else
									{
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
								}
								else
								{
									if (register_flag)
									{
										/*if (load_register_flag)
										{
											generate_aim_code("lw", "$s" + int_to_string(register_num), "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
											if (id_table[var_posi].addr_or_value)
											{
												generate_aim_code("lw", "$s" + int_to_string(register_num), "$s" + int_to_string(register_num), int_to_string(-id_table[var_posi].adr));
												generate_aim_code("lw", "$s" + int_to_string(register_num), "$s" + int_to_string(register_num), "0");
											}
											else
											{
												generate_aim_code("lw", "$s" + int_to_string(register_num), "$s" + int_to_string(register_num), int_to_string(-id_table[var_posi].adr));
											}
										}*/
										generate_aim_code("add", "$a0", "$s" + int_to_string(register_num), "$zero");
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
							}
							else if (fbs[g].sbs[h].new_codes[i].des.at(0) >= '0' && fbs[g].sbs[h].new_codes[i].des.at(0) <= '9')
							{
								generate_aim_code("li", "$a0", fbs[g].sbs[h].new_codes[i].des, "");
							}
							else
							{
								string temp_num_str;
								for (int k = 1; k < fbs[g].sbs[h].new_codes[i].des.size(); k++)
								{
									temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].des.at(k));
								}
								int temp_num = atoi(temp_num_str.c_str());
								if (temp_variables[temp_num] == 0)
								{
									temp_variables[temp_num] = fp_offsite;
									fp_offsite += 4;
									if (fp_offsite >= 500)
										generate_aim_code("subi", "$sp", "$sp", "4");
								}
								if (rigisters_condition[temp_rigisters[fbs[g].sbs[h].new_codes[i].des]] == fbs[g].sbs[h].new_codes[i].des)
								{
									generate_aim_code("add", "$a0", "$t" + int_to_string(temp_rigisters[fbs[g].sbs[h].new_codes[i].des] + 3), "$zero");
								}
								else
								{
									generate_aim_code("lw", "$a0", "$fp", int_to_string(-temp_variables[temp_num]));
								}
							//	generate_aim_code("lw", "$a0", "$fp", int_to_string(-temp_variables[temp_num]));
							}
							generate_aim_code("syscall", "", "", "");
						}
					}
					else if (fbs[g].sbs[h].new_codes[i].opr == "STORE" || fbs[g].sbs[h].new_codes[i].opr == "LOAD")
					{
						string des_register;
						string offsite_rigister;
						int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].src1);//查找到变量在符号表中的位置
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
						if (fbs[g].sbs[h].new_codes[i].src2.at(0) != '%' && (fbs[g].sbs[h].new_codes[i].src2.at(0) < '0' || fbs[g].sbs[h].new_codes[i].src2.at(0) > '9'))
						{
							int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].src2);//查找到变量在符号表中的位置
							bool register_flag = false;
							bool load_register_flag = false;
							
							int register_num = 0;
							if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
							{
							/*	if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
								{
									string occupied_var;
									if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
									{
										save_occupied(func_posi, register_num, occupied_var);
										var_rigisters[occupied_var] = -1;
										var_rigisters[id_table[var_posi].name] = register_num;
									}
									load_register_flag = true;
								}
								global_rigisters_condition[register_num] = id_table[var_posi].name;*/
								register_flag = true;
							}
							if (id_table[var_posi].obj == "const")
							{
								if (register_flag)
								{
									offsite_rigister = "$s" + int_to_string(register_num);
								/*	if (load_register_flag)
										generate_aim_code("li", offsite_rigister, int_to_string(id_table[var_posi].value),"");*/
								}
								else
								{
									generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
									offsite_rigister = "$t1";
								}
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								if (register_flag)
								{
									offsite_rigister = "$s" + int_to_string(register_num);
								/*	if (load_register_flag)
									{
										generate_aim_code("lw", offsite_rigister, "$fp", int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", offsite_rigister, offsite_rigister, "0");
									}*/
								}
								else
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
									if (id_table[var_posi].addr_or_value)
										generate_aim_code("lw", "$t1", "$t1", "0");
									offsite_rigister = "$t1";
								}
							}
							else
							{
								if (register_flag)
								{
									offsite_rigister = "$s" + int_to_string(register_num);
								/*	if (load_register_flag)
									{
										generate_aim_code("lw", offsite_rigister, "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
										generate_aim_code("lw", offsite_rigister, offsite_rigister, int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", offsite_rigister, offsite_rigister, "0");
									}*/
								}
								else
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
									generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
									if (id_table[var_posi].addr_or_value)
										generate_aim_code("lw", "$t1", "$t1", "0");
									offsite_rigister = "$t1";
								}
							}
						}
						else if (fbs[g].sbs[h].new_codes[i].src2.at(0) >= '0'&&fbs[g].sbs[h].new_codes[i].src2.at(0) <= '9')
						{
							generate_aim_code("li", "$t1", fbs[g].sbs[h].new_codes[i].src2, "");
							offsite_rigister = "$t1";
						}
						else if (fbs[g].sbs[h].new_codes[i].src2.at(0) == '%')
						{
							string temp_num_str;
							for (int k = 1; k < fbs[g].sbs[h].new_codes[i].src2.size(); k++)
							{
								temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].src2.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								if (fp_offsite >= 500)
									generate_aim_code("subi", "$sp", "$sp", "4");
							}
							if (rigisters_condition[temp_rigisters[fbs[g].sbs[h].new_codes[i].src2]] == fbs[g].sbs[h].new_codes[i].src2)
							{
								offsite_rigister = "$t" + int_to_string(temp_rigisters[fbs[g].sbs[h].new_codes[i].src2] + 3);
							}
							else
							{
								generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
								offsite_rigister = "$t1";
							}
						}
						generate_aim_code("sll", "$t1", offsite_rigister, "2");
						//						generate_aim_code("addi", "$t2", "$t0", fbs[g].sbs[h].new_codes[i].src2);
						//					generate_aim_code("sub","$t1","$zero","$t1");
						generate_aim_code("sub", "$t2", "$t0", "$t1");

						bool store_const = false;

						if (fbs[g].sbs[h].new_codes[i].des.at(0) != '%' && (fbs[g].sbs[h].new_codes[i].des.at(0) < '0' || fbs[g].sbs[h].new_codes[i].des.at(0) > '9'))//Store/load的des，我现在只需要将它的地址放在哪儿就好
						{
							int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].des);//查找到变量在符号表中的位置
							bool register_flag = false;
							bool load_register_flag = false;

							int register_num = 0;
							if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
							{
								/*if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
								{
									string occupied_var;
									if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
									{
										save_occupied(func_posi, register_num, occupied_var);
										var_rigisters[occupied_var] = -1;
										var_rigisters[id_table[var_posi].name] = register_num;
									}
									load_register_flag = true;
								}
								global_rigisters_condition[register_num] = id_table[var_posi].name;*/
								register_flag = true;
							}
							if (id_table[var_posi].obj == "const")
							{
								store_const = true;
								if (register_flag)
								{

									des_register = "$s" + int_to_string(register_num);
									if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
									{
										/*if (load_register_flag)
											generate_aim_code("li", des_register, int_to_string(id_table[var_posi].value), "");*/

									}
								}
								else
								{
									des_register = "$t1";
									if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
									{
										generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
									}
								}
							}
							else if (id_table[var_posi].obj == "function")
							{
								
								if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
									generate_aim_code("lw", "$t1", "$fp", "0");
								else
									generate_aim_code("addi", "$t1", "$fp", "0");
								des_register = "$t1";
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
							{
								//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
								if (register_flag)
								{
									des_register = "$s" + int_to_string(register_num);
									/*if (load_register_flag)
									{
										if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
										{
											if (id_table[var_posi].addr_or_value)
											{
												generate_aim_code("lw", des_register, "$fp", int_to_string(-id_table[var_posi].adr));
												generate_aim_code("lw", des_register, des_register, "0");
											}
											else
											{
												generate_aim_code("lw", des_register, "$fp", int_to_string(-id_table[var_posi].adr));
											}
										}
									}*/
								}
								else
								{
									if (id_table[var_posi].addr_or_value)
									{
										if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
										{
											generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
											generate_aim_code("lw", "$t1", "$t1", "0");
										}
										else
										{
											generate_aim_code("addi", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
											generate_aim_code("lw", "$t1", "$t1", "0");
										}
									}
									else
									{
										if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
											generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
										else
											generate_aim_code("addi", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
									}
									des_register = "$t1";
								}
							}
							else
							{
								if (register_flag)
								{
									des_register = "$s" + int_to_string(register_num);
									/*if (load_register_flag)
									{
										if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
										{
											generate_aim_code("lw", des_register, "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
											if (id_table[var_posi].addr_or_value)
											{
												generate_aim_code("lw", des_register, des_register, int_to_string(-id_table[var_posi].adr));
												generate_aim_code("lw", des_register, des_register, "0");
											}
											else
											{
												generate_aim_code("lw", des_register, des_register, int_to_string(-id_table[var_posi].adr));
											}
										}
									}*/
								}
								else
								{
									des_register = "$t1";
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
									if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
									{
										if (id_table[var_posi].addr_or_value)
										{
											generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
											generate_aim_code("lw", "$t1", "$t1", "0");
										}
										else
										{
											generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
										}
									}
									else
									{
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
							}
						}
						else if (fbs[g].sbs[h].new_codes[i].des.at(0) >= '0' && fbs[g].sbs[h].new_codes[i].des.at(0) <= '9')
						{
							generate_aim_code("li", "$t1", fbs[g].sbs[h].new_codes[i].des, "");
							des_register = "$t1";
							store_const = true;
						}
						else
						{
							string temp_num_str;
							for (int k = 1; k < fbs[g].sbs[h].new_codes[i].des.size(); k++)
							{
								temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].des.at(k));
							}
							int temp_num = atoi(temp_num_str.c_str());
							if (temp_variables[temp_num] == 0)
							{
								temp_variables[temp_num] = fp_offsite;
								fp_offsite += 4;
								if (fp_offsite >= 500)
									generate_aim_code("subi", "$sp", "$sp", "4");
							}
							des_register = "$t1";

							if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
							{
								if (rigisters_condition[temp_rigisters[fbs[g].sbs[h].new_codes[i].des]] == fbs[g].sbs[h].new_codes[i].des)
									des_register = "$t" + int_to_string(temp_rigisters[fbs[g].sbs[h].new_codes[i].des] + 3);
								else
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
									des_register = "$t1";
								}
							}
							else
							{
								temp_rigister++;
								if (temp_rigister == 7)
								{
									temp_rigister = 0;
								}
								des_register = "$t" + int_to_string(temp_rigister+3);
								if (rigisters_condition[temp_rigister] != "")
								{
									if (temp_if_saved(g,h,i,rigisters_condition[temp_rigister]))
									{
										string temp_num_str2;
										for (int k = 1; k < rigisters_condition[temp_rigister].size(); k++)
										{
											temp_num_str2.append(1, rigisters_condition[temp_rigister].at(k));
										}
										int temp_num2 = atoi(temp_num_str.c_str());
										generate_aim_code("sw",des_register,"$fp",int_to_string(-temp_variables[temp_num2]));
									}
								}
								rigisters_condition[temp_rigister] = fbs[g].sbs[h].new_codes[i].des;
								temp_rigisters[fbs[g].sbs[h].new_codes[i].des] = temp_rigister;
								//generate_aim_code("addi", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
							}
						}
						if (fbs[g].sbs[h].new_codes[i].opr == "STORE")
						{
							generate_aim_code("sw", des_register, "$t2", "0");
						}
						else
						{
							
							if (des_register == "$t1")
							{
								generate_aim_code("lw", "$t0", "$t2", "0");
								generate_aim_code("sw", "$t0", "$t1", "0");
							}
							else
								generate_aim_code("lw", des_register,"$t2","0");
						}
					}
					else
					{
						string src1_register;
						string src2_register;
						string des_register;

						if (fbs[g].sbs[h].new_codes[i].src1 != "")
						{
							if (fbs[g].sbs[h].new_codes[i].src1.at(0) != '%'&&fbs[g].sbs[h].new_codes[i].src1.at(0) != '@' && (fbs[g].sbs[h].new_codes[i].src1.at(0) < '0' || fbs[g].sbs[h].new_codes[i].src1.at(0) > '9'))
							{
								int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].src1);//查找到变量在符号表中的位置

								bool register_flag = false;
								bool load_register_flag = false;

								int register_num = 0;
								if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
								{
									/*if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
									{
										string occupied_var;
										if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
										{
											save_occupied(func_posi, register_num, occupied_var);
											var_rigisters[occupied_var] = -1;
											var_rigisters[id_table[var_posi].name] = register_num;
										}
										load_register_flag = true;
									}
									global_rigisters_condition[register_num] = id_table[var_posi].name;*/
									register_flag = true;
								}

								if (id_table[var_posi].obj == "const")
								{
									if (register_flag)
									{
										src1_register = "$s" + int_to_string(register_num);
										/*if (load_register_flag)
											generate_aim_code("li", src1_register, int_to_string(id_table[var_posi].value), "");*/
									}
									else{
										generate_aim_code("li", "$t0", int_to_string(id_table[var_posi].value), "");
										src1_register = "$t0";
									}
								}
								else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
								{
									if (register_flag)
									{
										src1_register = "$s" + int_to_string(register_num);
										/*if (load_register_flag)
										{
											generate_aim_code("lw", src1_register, "$fp", int_to_string(-id_table[var_posi].adr));
											if (id_table[var_posi].addr_or_value)
												generate_aim_code("lw", src1_register, src1_register, "0");
										}*/
									}
									else{
										generate_aim_code("lw", "$t0", "$fp", int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", "$t0", "$t0", "0");
										src1_register = "$t0";
									}
								}
								else
								{
									if (register_flag)
									{
										src1_register = "$s" + int_to_string(register_num);
										/*if (load_register_flag)
										{
											generate_aim_code("lw", src1_register, "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
											generate_aim_code("lw", src1_register, src1_register, int_to_string(-id_table[var_posi].adr));
											if (id_table[var_posi].addr_or_value)
												generate_aim_code("lw", src1_register, src1_register, "0");
										}*/
									}
									else
									{
										generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
										generate_aim_code("lw", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", "$t0", "$t0", "0");
										src1_register = "$t0";
									}
								}
							}
							else if (fbs[g].sbs[h].new_codes[i].src1.at(0) == '@')
							{
								generate_aim_code("lw", "$t0", "$sp", "0");
								src1_register = "$t0";
							}
							else if (fbs[g].sbs[h].new_codes[i].src1.at(0) >= '0'&&fbs[g].sbs[h].new_codes[i].src1.at(0) <= '9')
							{
								generate_aim_code("li", "$t0", fbs[g].sbs[h].new_codes[i].src1, "");
								src1_register = "$t0";
							}
							else if (fbs[g].sbs[h].new_codes[i].src1.at(0) == '%')
							{
								string temp_num_str;
								for (int k = 1; k < fbs[g].sbs[h].new_codes[i].src1.size(); k++)
								{
									temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].src1.at(k));
								}
								int temp_num = atoi(temp_num_str.c_str());
								if (temp_variables[temp_num] == 0)
								{
									temp_variables[temp_num] = fp_offsite;
									fp_offsite += 4;
									if (fp_offsite >= 500)
										generate_aim_code("subi", "$sp", "$sp", "4");
								}
								if (rigisters_condition[temp_rigisters[fbs[g].sbs[h].new_codes[i].src1]] == fbs[g].sbs[h].new_codes[i].src1)
									src1_register = "$t" + int_to_string(temp_rigisters[fbs[g].sbs[h].new_codes[i].src1]+3);
								else
								{
									generate_aim_code("lw","$t0","$fp",int_to_string(-temp_variables[temp_num]));
									src1_register = "$t0";
								}
							}
						}
						if (fbs[g].sbs[h].new_codes[i].src2 != "")
						{
							//很笨的一个方法，两个参数各自分析一下，代码重复了其实
							if (fbs[g].sbs[h].new_codes[i].src2.at(0) != '%' && (fbs[g].sbs[h].new_codes[i].src2.at(0) < '0' || fbs[g].sbs[h].new_codes[i].src2.at(0) > '9')&&fbs[g].sbs[h].new_codes[i].src2!="@")
							{
								int var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].src2);//查找到变量在符号表中的位置
								
								bool register_flag = false;
								bool load_register_flag = false;

								int register_num = 0;
								if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
								{
									/*if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
									{
										string occupied_var;
										if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
										{
											save_occupied(func_posi, register_num, occupied_var);
											var_rigisters[occupied_var] = -1;
											var_rigisters[id_table[var_posi].name] = register_num;
										}
										load_register_flag = true;
									}
									global_rigisters_condition[register_num] = id_table[var_posi].name;*/
									register_flag = true;
								}

								if (id_table[var_posi].obj == "const")
								{
									if (register_flag)
									{
										src2_register = "$s" + int_to_string(register_num);
										/*if (load_register_flag)
										{
											generate_aim_code("li", src2_register, int_to_string(id_table[var_posi].value), "");
										}*/
									}
									else
									{
										generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
										src2_register = "$t1";
									}
								}
								else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
								{
									if (register_flag)
									{
										src2_register = "$s" + int_to_string(register_num);
										/*if (load_register_flag)
										{
											generate_aim_code("lw", src2_register, "$fp", int_to_string(-id_table[var_posi].adr));
											if (id_table[var_posi].addr_or_value)
												generate_aim_code("lw", src2_register, src2_register, "0");
										}*/
									}
									else{
										generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", "$t1", "$t1", "0");
										src2_register = "$t1";
									}
								}
								else
								{
									if (register_flag)
									{
										src2_register = "$s" + int_to_string(register_num);
										/*if (load_register_flag)
										{
											generate_aim_code("lw", src2_register, "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
											generate_aim_code("lw", src2_register, src2_register, int_to_string(-id_table[var_posi].adr));
											if (id_table[var_posi].addr_or_value)
												generate_aim_code("lw", src2_register, src2_register, "0");
										}*/
									}
									else
									{
										generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
										generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi].adr));
										if (id_table[var_posi].addr_or_value)
											generate_aim_code("lw", "$t1", "$t1", "0");
										src2_register = "$t1";
									}
								}
							}
							else if (fbs[g].sbs[h].new_codes[i].src2.at(0) == '@')
							{
								generate_aim_code("lw", "$t1", "$sp", "0");
								src2_register = "$t1";
							}
							else if (fbs[g].sbs[h].new_codes[i].src2.at(0) >= '0'&&fbs[g].sbs[h].new_codes[i].src2.at(0) <= '9')
							{
								imediate_num = true;
								//			generate_aim_code("li", "$t1", "", fbs[g].sbs[h].new_codes[i].src2);
							}
							else if (fbs[g].sbs[h].new_codes[i].src2.at(0) == '%')
							{
								string temp_num_str;
								for (int k = 1; k < fbs[g].sbs[h].new_codes[i].src2.size(); k++)
								{
									temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].src2.at(k));
								}
								int temp_num = atoi(temp_num_str.c_str());
								if (temp_variables[temp_num] == 0)
								{
									temp_variables[temp_num] = fp_offsite;
									fp_offsite += 4;
									if (fp_offsite >= 500)
										generate_aim_code("subi", "$sp", "$sp", "4");
								}
								if (rigisters_condition[temp_rigisters[fbs[g].sbs[h].new_codes[i].src2]] == fbs[g].sbs[h].new_codes[i].src2)
									src2_register = "$t" + int_to_string(temp_rigisters[fbs[g].sbs[h].new_codes[i].src2] + 3);
								else
								{
									generate_aim_code("lw", "$t1", "$fp", int_to_string(-temp_variables[temp_num]));
									src2_register = "$t1";
								}
							}
						}
						if ((fbs[g].sbs[h].new_codes[i].opr == "ADD" || fbs[g].sbs[h].new_codes[i].opr == "SUB" || fbs[g].sbs[h].new_codes[i].opr == "MUL" || fbs[g].sbs[h].new_codes[i].opr == "DIV" || fbs[g].sbs[h].new_codes[i].opr == "OPP" || fbs[g].sbs[h].new_codes[i].opr == "ASSIGN") && fbs[g].sbs[h].new_codes[i].des != "")
						{
							if (fbs[g].sbs[h].new_codes[i].des.at(0) != '%' && (fbs[g].sbs[h].new_codes[i].des.at(0) < '0' || fbs[g].sbs[h].new_codes[i].des.at(0) > '9'))
							{
								int var_posi;
								if (id_table[func_posi].name == fbs[g].sbs[h].new_codes[i].des)
								{
									var_posi = func_posi;
								}
								else
									var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].des);//查找到变量在符号表中的位置
                       
								bool register_flag = false;
								bool load_register_flag = false;

								int register_num = 0;
								if ((register_num = if_in_map_or_not(id_table[var_posi].name, fbs[g].global_rigisters)) != -1)
								{
									/*if (if_in_map_or_not(id_table[var_posi].name, var_rigisters) == -1)
									{
										string occupied_var;
										if ((occupied_var = if_register_occupied(register_num, var_rigisters)) != "")
										{
											save_occupied(func_posi, register_num, occupied_var);
											var_rigisters[occupied_var] = -1;
											var_rigisters[id_table[var_posi].name] = register_num;
										}
										load_register_flag = true;
									}
									global_rigisters_condition[register_num] = id_table[var_posi].name;*/
									register_flag = true;
								}
								
								if (id_table[var_posi].obj == "function")
								{
									des_register = "$t2";
								}
								else
								{
									//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
									
									if (register_flag)
									{
										des_register = "$s" + int_to_string(register_num);
									}
									else
										des_register = "$t2";
								}
							}
							else
							{
								string temp_num_str;
								for (int k = 1; k < fbs[g].sbs[h].new_codes[i].des.size(); k++)
								{
									temp_num_str.append(1, fbs[g].sbs[h].new_codes[i].des.at(k));
								}
								int temp_num = atoi(temp_num_str.c_str());
								if (temp_variables[temp_num] == 0)
								{
									temp_variables[temp_num] = fp_offsite;
									fp_offsite += 4;
									if (fp_offsite >= 500)
										generate_aim_code("subi", "$sp", "$sp", "4");
								}
								temp_rigister++;
								if (temp_rigister == 7)
								{
									temp_rigister = 0;
								}
								des_register = "$t" + int_to_string(temp_rigister + 3);
								if (rigisters_condition[temp_rigister] != "")
								{
									if (temp_if_saved(g, h, i, rigisters_condition[temp_rigister]))
									{
										string temp_num_str2;
										for (int k = 1; k < rigisters_condition[temp_rigister].size(); k++)
										{
											temp_num_str2.append(1, rigisters_condition[temp_rigister].at(k));
										}
										int temp_num2 = atoi(temp_num_str.c_str());
										generate_aim_code("sw", des_register, "$fp", int_to_string(-temp_variables[temp_num2]));
									}
								}
								rigisters_condition[temp_rigister] = fbs[g].sbs[h].new_codes[i].des;
								temp_rigisters[fbs[g].sbs[h].new_codes[i].des] = temp_rigister;
							}
						}
						if (fbs[g].sbs[h].new_codes[i].opr == "ADD")
						{
							if (imediate_num)
								generate_aim_code("addi", des_register, src1_register, fbs[g].sbs[h].new_codes[i].src2);
							else
								generate_aim_code("add", des_register,src1_register, src2_register);

						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "SUB")
						{
							if (imediate_num)
								generate_aim_code("subi", des_register, src1_register, fbs[g].sbs[h].new_codes[i].src2);
							else
								generate_aim_code("sub", des_register, src1_register, src2_register);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "MUL")
						{
							if (imediate_num)
								generate_aim_code("mul", des_register, src1_register, fbs[g].sbs[h].new_codes[i].src2);
							else
								generate_aim_code("mulo", des_register, src1_register, src2_register);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "DIV")
						{
							if (imediate_num)
								generate_aim_code("div", des_register, src1_register, fbs[g].sbs[h].new_codes[i].src2);
							else
								generate_aim_code("div", des_register, src1_register, src2_register);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "OPP")
						{
							generate_aim_code("sub", des_register, "$zero", src1_register);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "ASSIGN")
						{
							generate_aim_code("add", des_register, "$zero", src1_register);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JGR")
						{
							if (imediate_num)
								generate_aim_code("bgt", src1_register, fbs[g].sbs[h].new_codes[i].src2, fbs[g].sbs[h].new_codes[i].des);
							else
								generate_aim_code("bgt", src1_register, src2_register, fbs[g].sbs[h].new_codes[i].des);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JGE")
						{
							if (imediate_num)
								generate_aim_code("bge", src1_register, fbs[g].sbs[h].new_codes[i].src2, fbs[g].sbs[h].new_codes[i].des);
							else
								generate_aim_code("bge", src1_register, src2_register, fbs[g].sbs[h].new_codes[i].des);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JLS")
						{
							if (imediate_num)
								generate_aim_code("blt", src1_register, fbs[g].sbs[h].new_codes[i].src2, fbs[g].sbs[h].new_codes[i].des);
							else
								generate_aim_code("blt", src1_register, src2_register, fbs[g].sbs[h].new_codes[i].des);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JLE")
						{
							if (imediate_num)
								generate_aim_code("ble", src1_register, fbs[g].sbs[h].new_codes[i].src2, fbs[g].sbs[h].new_codes[i].des);
							else
								generate_aim_code("ble", src1_register, src2_register, fbs[g].sbs[h].new_codes[i].des);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JEQ")
						{
							if (imediate_num)
								generate_aim_code("beq", src1_register, fbs[g].sbs[h].new_codes[i].src2, fbs[g].sbs[h].new_codes[i].des);
							else
								generate_aim_code("beq", src1_register, src2_register, fbs[g].sbs[h].new_codes[i].des);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JNE")
						{
							if (imediate_num)
								generate_aim_code("bne", src1_register, fbs[g].sbs[h].new_codes[i].src2, fbs[g].sbs[h].new_codes[i].des);
							else
								generate_aim_code("bne", src1_register, src2_register, fbs[g].sbs[h].new_codes[i].des);
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "LABEL")
						{
							generate_aim_code("LABEL", fbs[g].sbs[h].new_codes[i].des, "", "");
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "JMP")
						{
							if (fbs[g].sbs[h].new_codes[i].des.at(0) != '_')
							{
								int call_func_posi = find_func_proc_position(fbs[g].sbs[h].new_codes[i].des);
								int aim_code_num_temp = aim_code_num;
								//		
								int l;
								/*for (l = call_func_posi; id_table[l].obj == "const" || id_table[l].obj == "var"; l++)
								{
								}
								l--;
								call_sp_dcrease=id_table[l].adr;
								if (id_table[j].type == "array")
								call_sp_dcrease = id_table[j].adr + id_table[j].arrayinfo->size * 4;
								else
								call_sp_dcrease = id_table[j].adr + 4;*/

								for (l = call_func_posi + 1; id_table[l].obj == "const" || id_table[l].obj == "var"; l++)
								{
								}
								while (id_table[l].obj != "var"&&l != call_func_posi)
									l--;

								if (l == call_func_posi)
									call_sp_dcrease = (4 + id_table[call_func_posi].lev) * 4;
								else
								{
									call_sp_dcrease = id_table[l].adr;
									if (id_table[l].type == "array")
										call_sp_dcrease = id_table[l].adr + id_table[l].arrayinfo->size * 4;
									else
										call_sp_dcrease = id_table[l].adr + 4;
								}


								for (int k = 0; k < 7; k++)//保存临时寄存器和全局寄存器，直接写回栈中
								{
									if (rigisters_condition[k] != ""&&temp_if_saved(g, h, i, rigisters_condition[k]))
									{
										string temp_num_str;
										for (int j = 1; j < rigisters_condition[k].size(); j++)
										{
											temp_num_str.append(1, rigisters_condition[k].at(j));
										}
										int temp_num = atoi(temp_num_str.c_str());
										generate_aim_code("sw", "$t" + int_to_string(k + 3), "$fp", int_to_string(-temp_variables[temp_num]));
										temp_rigisters[rigisters_condition[k]] = 7;
										rigisters_condition[k] = "";
									}
								}
								for (int k = 0; k < 8; k++)
								{
									if (fbs[g].registers[k] != "")
									{
										int var_posi_temp = get_the_variable(func_posi, fbs[g].registers[k]);
										if (id_table[var_posi_temp].obj != "var")
										{
											continue;
										}
										if (if_should_save(g, h, i, fbs[g].registers[k]))
										{
											if (id_table[var_posi_temp].lev == (id_table[func_posi].lev + 1))
											{
												//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
												if (id_table[var_posi_temp].addr_or_value)
												{
													generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi_temp].adr));
													generate_aim_code("sw", "$s" + int_to_string(k), "$t1", "0");
												}
												else
												{
													generate_aim_code("sw", "$s" + int_to_string(k), "$fp", int_to_string(-id_table[var_posi_temp].adr));
												}
											}
											else
											{
												generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi_temp].lev - 1) * 4));
												if (id_table[var_posi_temp].addr_or_value)
												{
													generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi_temp].adr));
													generate_aim_code("sw", "$s" + int_to_string(k), "$t1", "0");
												}
												else
												{
													generate_aim_code("sw", "$s" + int_to_string(k), "$t1", int_to_string(-id_table[var_posi_temp].adr));

												}
											}
										}
									}
								}



								call_sp_dcrease = (call_sp_dcrease < 500) ? 500 : call_sp_dcrease;
								generate_aim_code("subi", "$sp", "$sp", int_to_string(call_sp_dcrease));
								generate_aim_code("jal", fbs[g].sbs[h].new_codes[i].des, "", "");
								call_sp_dcrease = 0; for (int k = 0; k < 8; k++)
								{
									if (fbs[g].registers[k] != "")
									{
										int var_posi_temp = get_the_variable(func_posi, fbs[g].registers[k]);
										if (id_table[var_posi_temp].obj != "var")
										{
											if (id_table[var_posi_temp].obj == "const")
											{
												generate_aim_code("li", "$s" + int_to_string(k), int_to_string(id_table[var_posi_temp].value), "");
											}
											continue;
										}
										if (if_should_load(g, h, i, fbs[g].registers[k]))
										{
											if (id_table[var_posi_temp].lev == (id_table[func_posi].lev + 1))
											{
												//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
												if (id_table[var_posi_temp].addr_or_value)
												{
													generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi_temp].adr));
													generate_aim_code("lw", "$s" + int_to_string(k), "$t1", "0");
												}
												else
												{
													generate_aim_code("lw", "$s" + int_to_string(k), "$fp", int_to_string(-id_table[var_posi_temp].adr));
												}
											}
											else
											{
												generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi_temp].lev - 1) * 4));
												if (id_table[var_posi_temp].addr_or_value)
												{
													generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi_temp].adr));
													generate_aim_code("lw", "$s" + int_to_string(k), "$t1", "0");
												}
												else
												{
													generate_aim_code("lw", "$s" + int_to_string(k), "$t1", int_to_string(-id_table[var_posi_temp].adr));

												}
											}
										}
									}
								}
							}
							else
								generate_aim_code("jal", fbs[g].sbs[h].new_codes[i].des, "", "");
							
							
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "PUSH")
						{
							call_sp_dcrease -= 4;
							generate_aim_code("sw", src1_register, "$sp", int_to_string(call_sp_dcrease));
						}
						else if (fbs[g].sbs[h].new_codes[i].opr == "RETURN")
						{
							for (int k = 0; k < 8; k++)
							{
								if (fbs[g].registers[k] != "")
								{
									int var_posi_temp = get_the_variable(func_posi, fbs[g].registers[k]);
									if (id_table[var_posi_temp].obj != "var")
									{
										continue;
									}
									if (if_should_save(g, h, i,fbs[g].registers[k]))
									{
										if (id_table[var_posi_temp].lev == (id_table[func_posi].lev + 1))
										{
											//                      generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi].adr));
											if (id_table[var_posi_temp].addr_or_value)
											{
												generate_aim_code("lw", "$t1", "$fp", int_to_string(-id_table[var_posi_temp].adr));
												generate_aim_code("sw", "$s" + int_to_string(k), "$t1", "0");
											}
											else
											{
												generate_aim_code("sw", "$s" + int_to_string(k), "$fp", int_to_string(-id_table[var_posi_temp].adr));
											}
										}
										else
										{
											generate_aim_code("lw", "$t1", "$fp", int_to_string(-(4 + id_table[var_posi_temp].lev - 1) * 4));
											if (id_table[var_posi_temp].addr_or_value)
											{
												generate_aim_code("lw", "$t1", "$t1", int_to_string(-id_table[var_posi_temp].adr));
												generate_aim_code("sw", "$s" + int_to_string(k), "$t1", "0");
											}
											else
											{
												generate_aim_code("sw", "$s" + int_to_string(k), "$t1", int_to_string(-id_table[var_posi_temp].adr));

											}
										}
									}
								}
							}
							generate_aim_code("lw", "$sp", "$fp", int_to_string(-4));
							generate_aim_code("lw", "$fp", "$fp", int_to_string(-8));
							generate_aim_code("lw", "$ra", "$sp", int_to_string(-12));
							generate_aim_code("jr", "$ra", "", "");
						}
						if ((fbs[g].sbs[h].new_codes[i].opr == "ADD" || fbs[g].sbs[h].new_codes[i].opr == "SUB" || fbs[g].sbs[h].new_codes[i].opr == "MUL" || fbs[g].sbs[h].new_codes[i].opr == "DIV" || fbs[g].sbs[h].new_codes[i].opr == "OPP" || fbs[g].sbs[h].new_codes[i].opr == "ASSIGN") && fbs[g].sbs[h].new_codes[i].des != ""&&des_register == "$t2")
						{
							if (fbs[g].sbs[h].new_codes[i].des.at(0) != '%' && (fbs[g].sbs[h].new_codes[i].des.at(0) < '0' || fbs[g].sbs[h].new_codes[i].des.at(0) > '9'))
							{
								int var_posi;
								if (id_table[func_posi].name == fbs[g].sbs[h].new_codes[i].des)
								{
									var_posi = func_posi;
								}
								else
									var_posi = get_the_variable(func_posi, fbs[g].sbs[h].new_codes[i].des);//查找到变量在符号表中的位置
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
										generate_aim_code("sw", "$t2", "$t1", "0");
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
						}
					}
					i++;
				}

			}
		}
	}
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
			int temp_variables[200] = {0};
			int call_sp_dcrease = 0;
	//		
			int j;
			int fp_offsite;
			for (j = func_posi + 1; id_table[j].obj == "const" || id_table[j].obj == "var"; j++)
			{
			}
			while (id_table[j].obj != "var"&&j!=func_posi)
			j--;
			
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
					for (int k = 0; k < id_table[func_posi].lev&&k<id_table[call_func_posi].lev; k++)
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
							
						}
						else
						{
							generate_aim_code("lw", "$t0", "$fp", int_to_string(-(4 + id_table[var_posi].lev - 1) * 4));
							generate_aim_code("addi", "$t0", "$t0", int_to_string(-id_table[var_posi].adr));
					//		generate_aim_code("addi", "$t2", "$t0", codes[i].src2);
						}
						if (id_table[var_posi].addr_or_value)
						{
							generate_aim_code("lw", "$t0", "$t0", "0");
						}
					}
					if (codes[i].src2.at(0) >= '0'&&codes[i].src2.at(0) <= '9')
					{
						generate_aim_code("li", "$t1",codes[i].src2,"");
					}
					else  if (codes[i].src2.at(0) == '%')
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
					else
					{
						int var_posi = get_the_variable(func_posi, codes[i].src2);//查找到变量在符号表中的位置
						if (id_table[var_posi].obj == "const")
						{
							generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
						}
						else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
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
					generate_aim_code("sll", "$t1", "$t1", "2");
					generate_aim_code("sub", "$t2", "$t0", "$t1");
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
					if (codes[i].src2 == "string")
					{
						generate_aim_code("la","$a0",codes[i].des,"");
						generate_aim_code("li","$v0","4","");
						generate_aim_code("syscall","","","");
					}
					else
					{
						if (codes[i].src2 == "integersym")
							generate_aim_code("li", "$v0", "1", "");

						else if (codes[i].src2 == "charsym")
							generate_aim_code("li", "$v0", "11", "");


						if (codes[i].des != "" && (codes[i].des.at(0) != '%' && (codes[i].des.at(0) < '0' || codes[i].des.at(0) > '9')))//Store/load的des，我现在只需要将它的地址放在哪儿就好
						{
							int var_posi = get_the_variable(func_posi, codes[i].des);//查找到变量在符号表中的位置

							if (id_table[var_posi].obj == "const")
							{
								generate_aim_code("li","$a0",int_to_string(id_table[var_posi].value),"");
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
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
						else if (codes[i].des.at(0) >= '0' && codes[i].des.at(0) <= '9')
						{
							generate_aim_code("li", "$a0", codes[i].des, "");
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
						if (id_table[var_posi].obj == "const")
						{
							generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
						}
						else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
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
				    generate_aim_code("sll","$t1","$t1","2");
//						generate_aim_code("addi", "$t2", "$t0", codes[i].src2);
//					generate_aim_code("sub","$t1","$zero","$t1");
				    generate_aim_code("sub", "$t2", "$t0", "$t1");

					bool store_const = false;

					if (codes[i].des.at(0) != '%' && (codes[i].des.at(0) < '0' || codes[i].des.at(0) > '9'))//Store/load的des，我现在只需要将它的地址放在哪儿就好
					{
						int var_posi = get_the_variable(func_posi, codes[i].des);//查找到变量在符号表中的位置
						if (id_table[var_posi].obj == "const")
						{
							store_const = true;
							generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
						}
						else if (id_table[var_posi].obj == "function")
						{
							generate_aim_code("addi", "$t1", "$fp", "0");
						}
						else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
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
					else if (codes[i].des.at(0) >= '0' && codes[i].des.at(0) <= '9')
					{
						generate_aim_code("li", "$t1", codes[i].des, "");
						store_const = true;
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
						if (!store_const)
						{
							generate_aim_code("lw", "$t0", "$t1", "0");
							generate_aim_code("sw", "$t0", "$t2", "0");
						}
						else
							generate_aim_code("sw", "$t1", "$t2", "0");
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
							if (id_table[var_posi].obj == "const")
							{
								generate_aim_code("li", "$t0", int_to_string(id_table[var_posi].value), "");
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
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
							if (id_table[var_posi].obj == "const")
							{
								generate_aim_code("li", "$t1", int_to_string(id_table[var_posi].value), "");
							}
							else if (id_table[var_posi].lev == (id_table[func_posi].lev + 1))
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
							generate_aim_code("sub", "$t2", "$zero", "$t0");
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
							int call_func_posi = find_func_proc_position(codes[i].des);
							int aim_code_num_temp = aim_code_num;
							//		
							int l;
							/*for (l = call_func_posi; id_table[l].obj == "const" || id_table[l].obj == "var"; l++)
							{
							}
							l--;
							call_sp_dcrease=id_table[l].adr;
							if (id_table[j].type == "array")
								call_sp_dcrease = id_table[j].adr + id_table[j].arrayinfo->size * 4;
							else
								call_sp_dcrease = id_table[j].adr + 4;*/
							
							for (l =call_func_posi + 1; id_table[l].obj == "const" || id_table[l].obj == "var"; l++)
							{
							}
							while (id_table[l].obj != "var"&&l != call_func_posi)
							l--;
			
							if (l == call_func_posi)
								call_sp_dcrease = (4 + id_table[call_func_posi].lev) * 4;
							else
							{
								call_sp_dcrease = id_table[l].adr;
								if (id_table[l].type == "array")
									call_sp_dcrease = id_table[l].adr + id_table[l].arrayinfo->size * 4;
								else
									call_sp_dcrease = id_table[l].adr + 4;
							}
							
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
	ofstream fout("test.asm");
	fout << ".data" << endl;
	for (int i = 0; i < string_num; i++)
	{
		fout << generate_string_Label(i) << ": " << ".asciiz" << " \"" << string_pool[i]<<"\""<<endl;
	}
	fout << ".text" << endl;
	for (int i = 0; i < aim_code_num; i++)
	{
		
		if (aim_codes[i].opr == "addi" || aim_codes[i].opr == "subi" || aim_codes[i].opr == "mul" || aim_codes[i].opr == "mulo" || aim_codes[i].opr == "div" || aim_codes[i].opr == "add" || aim_codes[i].opr == "sub" ||aim_codes[i].opr=="sll")
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
		else if (aim_codes[i].opr == "li" || aim_codes[i].opr == "la")
		{
			fout << aim_codes[i].opr << " " << aim_codes[i].des << "," << aim_codes[i].src1<<endl;
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
