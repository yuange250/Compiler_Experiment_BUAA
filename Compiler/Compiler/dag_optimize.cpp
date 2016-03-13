#include "stdafx.h"
#include "globals.h"
function_block fbs[20];
int fb_nums = 0;
void mark_standard_block(){
//	int fb_nums=0;
//	int sb_nums = 0;
//	int sb_codes_num = 0;
	for (int i = 0; i < code_index; i++,fb_nums++)
	{
		if (codes[i].opr == "PLABEL")
		{
			fbs[fb_nums].sb_nums = 0;
			int j;
			for (j = i; codes[j].opr != "RETURN"&&j<code_index; fbs[fb_nums].sb_nums++)
			{
				fbs[fb_nums].sbs[fbs[fb_nums].sb_nums].codes_num = 0;
				fbs[fb_nums].sbs[fbs[fb_nums].sb_nums].codes[fbs[fb_nums].sbs[fbs[fb_nums].sb_nums].codes_num++] = codes[j];
				if (codes[j].opr == "JMP" || codes[j].opr == "JEQ" || codes[j].opr == "JNE" || codes[j].opr == "JLS" || codes[j].opr == "JLE" || codes[j].opr == "JGR" || codes[j].opr == "JGE")
				{
					j++;
					continue;
				}
				int k;
				for (k = j + 1; codes[k].opr != "LABEL" && codes[k].opr != "RETURN"&&k<code_index; k++)
				{
					fbs[fb_nums].sbs[fbs[fb_nums].sb_nums].codes[fbs[fb_nums].sbs[fbs[fb_nums].sb_nums].codes_num++] = codes[k];
					if (codes[k].opr == "JMP" || codes[k].opr == "JEQ" || codes[k].opr == "JNE" || codes[k].opr == "JLS" || codes[k].opr == "JLE" || codes[k].opr == "JGR" || codes[k].opr == "JGE")
					{
						k++;
						break;
					}
				}
				j = k;
			}
			i = j;
		}
		for (int j = 0; j < fbs[fb_nums].sb_nums; j++)
		{
			if (fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JMP"
				|| fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JEQ"
				|| fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JNE"
				|| fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JLS"
				|| fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JLE"
				|| fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JGR"
				|| fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].opr == "JGE")
			{
				for (int k = 0; k < fbs[fb_nums].sb_nums; k++)
				{
					if (fbs[fb_nums].sbs[k].codes[0].opr == "LABEL"
						&&fbs[fb_nums].sbs[j].codes[fbs[fb_nums].sbs[j].codes_num - 1].des == fbs[fb_nums].sbs[k].codes[0].des
						&&k != j + 1)
					{
						fbs[fb_nums].sbs[j].backward_blocks_num[fbs[fb_nums].sbs[j].bb_num++] = k;
						fbs[fb_nums].sbs[k].forward_blocks_num[fbs[fb_nums].sbs[k].fb_num++] = j;
						break;
					}
				}
			}
			if ((j + 1) < fbs[fb_nums].sb_nums)
			{
				fbs[fb_nums].sbs[j].backward_blocks_num[fbs[fb_nums].sbs[j].bb_num++] = j + 1;
				fbs[fb_nums].sbs[j + 1].forward_blocks_num[fbs[fb_nums].sbs[j + 1].fb_num++] = j;
			}
		}
	}
	
}
bool ifin_array(string name,string names[],int length)
{
	for (int i = 0; i < length; i++)
	{
		if (name == names[i])
			return true;
	}
	return false;
}

void DAG_sblock_optimize(int fb_num,int sb_num)
{
	map<string, int> vars_map;
	DAG_node dnodes[100];
	int nodes_num = 1;
	bool store_flag = true;//数组的存取的一个标志，连续的两次取操作中间不可以夹杂存操作
	Stack_node output_stack[100];
	int take_off_nums_stack[100] = {0};
	int stack_node_num = 0;
	for (int i = 0; i < fbs[fb_num].sbs[sb_num].codes_num; i++)
	{
		int node_no = 0;
		int left_child_no = 0;
		int right_child_no = 0;
		for (map<string, int>::iterator it = vars_map.begin(); it != vars_map.end()&&node_no==0; it--)
		{
			if (fbs[fb_num].sbs[sb_num].codes[i].opr == "ADD" || fbs[fb_num].sbs[sb_num].codes[i].opr == "MUL")
			{
				if (dnodes[it->second].text == fbs[fb_num].sbs[sb_num].codes[i].opr)
				{
					if (((
						ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src1, dnodes[dnodes[it->second].left_child_num].vars, dnodes[dnodes[it->second].left_child_num].vars_num) &&
						ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src2, dnodes[dnodes[it->second].right_child_num].vars, dnodes[dnodes[it->second].right_child_num].vars_num))
						|| (
						ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src1, dnodes[dnodes[it->second].right_child_num].vars, dnodes[dnodes[it->second].right_child_num].vars_num) &&
						ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src2, dnodes[dnodes[it->second].left_child_num].vars, dnodes[dnodes[it->second].left_child_num].vars_num)))
						&& (vars_map[dnodes[it->second].vars[0]] == it->second || dnodes[it->second].vars[0].at(0) == '%'))
					{
						vars_map[fbs[fb_num].sbs[sb_num].codes[i].des] = it->second;
						node_no = it->second;
						dnodes[it->second].vars[dnodes[it->second].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].des;
						
						if (fbs[fb_num].sbs[sb_num].codes[i].des.at(0) != '%'&&!ifin_array(fbs[fb_num].sbs[sb_num].codes[i].des, dnodes[it->second].vars, dnodes[it->second].vars_num-1))
						{
							output_stack[stack_node_num].var_name = fbs[fb_num].sbs[sb_num].codes[i].des;
							output_stack[stack_node_num++].node_num = it->second;
						}

						//						dnodes[dnodes[it->second].left_child_num].fathers_num++;
//						dnodes[dnodes[it->second].right_child_num].fathers_num++;
					}
				}
			}
			else if (fbs[fb_num].sbs[sb_num].codes[i].opr == "DIV" || fbs[fb_num].sbs[sb_num].codes[i].opr == "SUB")
			{
				if (dnodes[it->second].text == fbs[fb_num].sbs[sb_num].codes[i].opr)
				{
					if ((ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src1, dnodes[dnodes[it->second].left_child_num].vars, dnodes[dnodes[it->second].left_child_num].vars_num) &&
						ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src2, dnodes[dnodes[it->second].right_child_num].vars, dnodes[dnodes[it->second].right_child_num].vars_num)) 
						&& (vars_map[dnodes[it->second].vars[0]] == it->second || dnodes[it->second].vars[0].at(0) == '%'))
					{
						vars_map[fbs[fb_num].sbs[sb_num].codes[i].des] = it->second;
						node_no = it->second;
						dnodes[it->second].vars[dnodes[it->second].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].des;
//						dnodes[it->second].vars[dnodes[it->second].vars_num++] = codes[i].des;
//						dnodes[dnodes[it->second].left_child_num].fathers_num++;
						
						if (fbs[fb_num].sbs[sb_num].codes[i].des.at(0) != '%'&&!ifin_array(fbs[fb_num].sbs[sb_num].codes[i].des, dnodes[it->second].vars, dnodes[it->second].vars_num - 1))
						{
							output_stack[stack_node_num].var_name = fbs[fb_num].sbs[sb_num].codes[i].des;
							output_stack[stack_node_num++].node_num = it->second;
						}

					}
				}

			}
			else if (fbs[fb_num].sbs[sb_num].codes[i].opr == "LOAD"&&store_flag)
			{
				if (dnodes[it->second].text == fbs[fb_num].sbs[sb_num].codes[i].opr)
				{
					if ((ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src1, dnodes[dnodes[it->second].left_child_num].vars, dnodes[dnodes[it->second].left_child_num].vars_num) &&
						ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src2, dnodes[dnodes[it->second].right_child_num].vars, dnodes[dnodes[it->second].right_child_num].vars_num)) && (vars_map[dnodes[it->second].vars[0]] == it->second || dnodes[it->second].vars[0].at(0) == '%'))
					{
						vars_map[fbs[fb_num].sbs[sb_num].codes[i].des] = it->second;
						node_no = it->second;
						dnodes[it->second].vars[dnodes[it->second].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].des;

						if (fbs[fb_num].sbs[sb_num].codes[i].des.at(0) != '%')
						{
							output_stack[stack_node_num].var_name = fbs[fb_num].sbs[sb_num].codes[i].des;
							output_stack[stack_node_num++].node_num = it->second;
						}


//						dnodes[dnodes[it->second].left_child_num].fathers_num++;
//						dnodes[dnodes[it->second].right_child_num].fathers_num++;
					}
				}
			}
			else if (fbs[fb_num].sbs[sb_num].codes[i].opr == "OPP")
			{
				if (dnodes[it->second].text == fbs[fb_num].sbs[sb_num].codes[i].opr)
				{
					if ((ifin_array(fbs[fb_num].sbs[sb_num].codes[i].src1, dnodes[dnodes[it->second].left_child_num].vars, dnodes[dnodes[it->second].left_child_num].vars_num)) && (vars_map[dnodes[it->second].vars[0]] == it->second || dnodes[it->second].vars[0].at(0) == '%'))
					{
						vars_map[fbs[fb_num].sbs[sb_num].codes[i].des] = it->second;
						node_no = it->second;
						dnodes[it->second].vars[dnodes[it->second].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].des;


						if (fbs[fb_num].sbs[sb_num].codes[i].des.at(0) != '%')
						{
							output_stack[stack_node_num].var_name = fbs[fb_num].sbs[sb_num].codes[i].des;
							output_stack[stack_node_num++].node_num = it->second;
						}
//						dnodes[dnodes[it->second].left_child_num].fathers_num++;
//						dnodes[dnodes[it->second].right_child_num].fathers_num++;
					}
				}
			}
		}
		if (node_no == 0)
		{
			if (fbs[fb_num].sbs[sb_num].codes[i].opr == "ASSIGN")
			{
				map<string, int>::iterator it = vars_map.find(fbs[fb_num].sbs[sb_num].codes[i].src1);
				if (it != vars_map.end() && (vars_map[dnodes[it->second].vars[0]] == it->second || dnodes[it->second].vars[0].at(0)=='%'))
				{
					left_child_no = it->second;
	//				dnodes[it->second].fathers_num++;
				}
				else
				{
					dnodes[nodes_num].text = fbs[fb_num].sbs[sb_num].codes[i].src1;
					dnodes[nodes_num].left_child_num = 0;
					dnodes[nodes_num].right_child_num = 0;
					dnodes[nodes_num].vars_num = 0;
					dnodes[nodes_num].enable = true;
					dnodes[nodes_num].vars[dnodes[nodes_num].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].src1;
					dnodes[nodes_num].fathers_num=0;
					left_child_no = nodes_num;
					vars_map[fbs[fb_num].sbs[sb_num].codes[i].src1] = nodes_num++;
				}
				/*
				dnodes[nodes_num].text = codes[i].opr;
				dnodes[nodes_num].left_child_num = left_child_no;
				dnodes[nodes_num].right_child_num = 0;
				dnodes[nodes_num].vars_num = 0;
				*/

				dnodes[left_child_no].vars[dnodes[left_child_no].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].des;
				
				output_stack[stack_node_num].var_name = fbs[fb_num].sbs[sb_num].codes[i].des;
				output_stack[stack_node_num++].node_num = left_child_no;

				vars_map[fbs[fb_num].sbs[sb_num].codes[i].des] = left_child_no;

			}
			else{
				if (fbs[fb_num].sbs[sb_num].codes[i].opr == "STORE")
					store_flag = false;
				if (fbs[fb_num].sbs[sb_num].codes[i].src1 != "")
				{
					map<string, int>::iterator it = vars_map.find(fbs[fb_num].sbs[sb_num].codes[i].src1);
					if (it != vars_map.end() && (vars_map[dnodes[it->second].vars[0]] == it->second || dnodes[it->second].vars[0].at(0) == '%'))
					{
						left_child_no = it->second;
						dnodes[it->second].fathers_num++;
					}
					else
					{
						dnodes[nodes_num].text = fbs[fb_num].sbs[sb_num].codes[i].src1;
						dnodes[nodes_num].left_child_num = 0;
						dnodes[nodes_num].right_child_num = 0;
						dnodes[nodes_num].vars_num = 0;
						dnodes[nodes_num].fathers_num = 1;
						dnodes[nodes_num].enable = true;
						dnodes[nodes_num].vars[dnodes[nodes_num].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].src1;
						left_child_no = nodes_num;
						vars_map[fbs[fb_num].sbs[sb_num].codes[i].src1] = nodes_num++;
					}
				}
				else
					left_child_no = 0;
				if (fbs[fb_num].sbs[sb_num].codes[i].src2 != "")
				{
					map<string, int>::iterator it2 = vars_map.find(fbs[fb_num].sbs[sb_num].codes[i].src2);
					if (it2 != vars_map.end())
					{
						right_child_no = it2->second;
						dnodes[it2->second].fathers_num++;
					}
					else
					{
						dnodes[nodes_num].text = fbs[fb_num].sbs[sb_num].codes[i].src2;
						dnodes[nodes_num].left_child_num = 0;
						dnodes[nodes_num].right_child_num = 0;
						dnodes[nodes_num].vars_num = 0;
						dnodes[nodes_num].enable = true;
						dnodes[nodes_num].fathers_num=1;
						dnodes[nodes_num].vars[dnodes[nodes_num].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].src2;
						right_child_no = nodes_num;
						vars_map[fbs[fb_num].sbs[sb_num].codes[i].src2] = nodes_num++;
					}
				}
				else
					right_child_no = 0;
				dnodes[nodes_num].text = fbs[fb_num].sbs[sb_num].codes[i].opr;
				dnodes[nodes_num].left_child_num = left_child_no;
				dnodes[nodes_num].right_child_num = right_child_no;
				dnodes[nodes_num].vars_num = 0;
				dnodes[nodes_num].enable = true;
				dnodes[nodes_num].fathers_num = 0;
				dnodes[nodes_num].vars[dnodes[nodes_num].vars_num++] = fbs[fb_num].sbs[sb_num].codes[i].des;
				
				output_stack[stack_node_num].var_name = fbs[fb_num].sbs[sb_num].codes[i].des;
				output_stack[stack_node_num++].node_num = nodes_num;

				vars_map[fbs[fb_num].sbs[sb_num].codes[i].des] = nodes_num++;
			}
		}

	}
	/*for (int remove_num = 1; remove_num != nodes_num;)
	{
		for (int j = nodes_num-1; j > 0; j--)
		{
			do{
				if (dnodes[j].fathers_num == 0)
				{
					if (dnodes[j].enable)
					{
						output_stack[stack_node_num].var_name = dnodes[j].vars[0];
						output_stack[stack_node_num++].node_num = j;
						for (int k = 1; k < dnodes[j].vars_num; k++)
						{
							if (dnodes[j].vars[k].at(0) != '%')
							{
								output_stack[stack_node_num].var_name = dnodes[j].vars[k];
								output_stack[stack_node_num++].node_num = j;
							}
						}
						dnodes[dnodes[j].left_child_num].fathers_num--;
						dnodes[dnodes[j].right_child_num].fathers_num--;
						remove_num++;
						dnodes[j].enable = false;
						j = dnodes[j].left_child_num;
					}
					else
						break;
				}
				else
				{
					j = nodes_num - 1;
					break;
				}
			} while (true);
		}
	}*/
	for (int i = 0; i < stack_node_num; i++)
	{
		/*
		 if (dnodes[output_stack[i].node_num].left_child_num == 0 && dnodes[output_stack[i].node_num].right_child_num == 0)
		 {
		 if (output_stack[i].var_name != dnodes[output_stack[i].node_num].vars[0])
		 {
		 fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = "ASSIGN";
		 fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[output_stack[i].node_num].vars[0];
		 fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = "";
		 fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].des = output_stack[i].var_name;
		 }
		 }
		 else
		 {
		 */
		if (output_stack[i].var_name != dnodes[output_stack[i].node_num].vars[0])
		{
			if (dnodes[output_stack[i].node_num].vars[0] == "@"&&output_stack[i].var_name != dnodes[output_stack[i].node_num].vars[1])
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = "ASSIGN";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[output_stack[i].node_num].vars[1];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = output_stack[i].var_name;
			}
			else
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = "ASSIGN";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[output_stack[i].node_num].vars[0];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = output_stack[i].var_name;
			}
		}
		else
		{
			if (dnodes[output_stack[i].node_num].text == "ADD" 
				|| dnodes[output_stack[i].node_num].text == "SUB"
				|| dnodes[output_stack[i].node_num].text == "DIV"
				|| dnodes[output_stack[i].node_num].text == "MUL"
				|| dnodes[output_stack[i].node_num].text == "JGR"
				|| dnodes[output_stack[i].node_num].text == "JGE"
				|| dnodes[output_stack[i].node_num].text == "JLS"
				|| dnodes[output_stack[i].node_num].text == "JLE"
				|| dnodes[output_stack[i].node_num].text == "JEQ"
				|| dnodes[output_stack[i].node_num].text == "JNE"
				|| dnodes[output_stack[i].node_num].text == "STORE"
				|| dnodes[output_stack[i].node_num].text == "LOAD"
				)
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = dnodes[output_stack[i].node_num].text;
				if (dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[0]!="@")
					fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[0];
				else
					fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[1];
				if (dnodes[dnodes[output_stack[i].node_num].right_child_num].vars[0] != "@")
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = dnodes[dnodes[output_stack[i].node_num].right_child_num].vars[0];
				else
					fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = dnodes[dnodes[output_stack[i].node_num].right_child_num].vars[1];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = output_stack[i].var_name;
			}
			else if (dnodes[output_stack[i].node_num].text == "OPP")
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = dnodes[output_stack[i].node_num].text;
				if (dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[0] != "@")
					fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[0];
				else
					fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[1];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = output_stack[i].var_name;
			}
			else if (dnodes[output_stack[i].node_num].text == "JMP"
				|| dnodes[output_stack[i].node_num].text == "LABEL"
				|| dnodes[output_stack[i].node_num].text == "PLABEL"
				)
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = dnodes[output_stack[i].node_num].text;
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = output_stack[i].var_name;
			}
			else if (dnodes[output_stack[i].node_num].text == "READ"
				|| dnodes[output_stack[i].node_num].text == "WRITE")
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = dnodes[output_stack[i].node_num].text;
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = dnodes[dnodes[output_stack[i].node_num].right_child_num].text;
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = output_stack[i].var_name;
			}
			else if (dnodes[output_stack[i].node_num].text == "CALL"
				|| dnodes[output_stack[i].node_num].text == "PUSH")
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = dnodes[output_stack[i].node_num].text;
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[0];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = "";
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = "";
			}
			else if (dnodes[output_stack[i].node_num].text == "PUSHA")
			{
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].opr = dnodes[output_stack[i].node_num].text;
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src1 = dnodes[dnodes[output_stack[i].node_num].left_child_num].vars[0];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num].src2 = dnodes[dnodes[output_stack[i].node_num].right_child_num].vars[0];
				fbs[fb_num].sbs[sb_num].new_codes[fbs[fb_num].sbs[sb_num].new_codes_num++].des = "";
			}
		}
	}
}
void DAG_optimize()
{
	mark_standard_block();
	for (int i = 0; i < fb_nums; i++)
	{
		for (int j = 0; j < fbs[i].sb_nums; j++)
		{
			DAG_sblock_optimize(i, j);
		}
	}
	//code_index = 0;
	for (int i = 0; i < fb_nums-1; i++)
	{
		int j;
		/*for (j = 0; j < fbs[i].sb_nums; j++)
		{
			for (int k = 0; k < fbs[i].sbs[j].new_codes_num; k++)
			{
				codes[code_index++] = fbs[i].sbs[j].new_codes[k];
			}
		}*/
		fbs[i].sbs[fbs[i].sb_nums - 1].new_codes[fbs[i].sbs[fbs[i].sb_nums - 1].new_codes_num++].opr = "RETURN";
	/*	codes[code_index].opr = "RETURN";
		codes[code_index].src1 = "";
		codes[code_index].src2 = "";
		codes[code_index++].des = "";*/
	}
}