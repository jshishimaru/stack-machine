#ifndef IO_HPP
#define IO_HPP

#include <bits/stdc++.h>
using namespace std;

struct Instruction
{
	string name;
	int operand;
};

struct var_instruction
{
	string name;
	string var_name;
};

int hashString(string str)
{
	std::hash<std::string> hash_fn;
	int hash16 = hash_fn(str);
	return hash16;
}

class IO
{
public:
	vector<Instruction> input;
	map<string, int> label_begins;
	map<int, string> labels;
	vector<Instruction> *memory;
	vector<int> *dataStack;

	IO()
	{
		this->input = vector<Instruction>(1024);
		this->label_begins = map<string, int>();
	}

	void readInput(string filename, vector<Instruction> &mem)
	{
		ifstream file(filename);
		if (file.is_open())
		{
			string is_branch_inst;
			int ind = 0;
			while (getline(file, is_branch_inst))
			{
				while (is_branch_inst[0] == 9)
					is_branch_inst.erase(0, 1);
				while (is_branch_inst[0] == 32)
					is_branch_inst.erase(0, 1);
				if (is_branch_inst.size() == 0)
					continue;
				if (is_branch_inst[0] == '.')
				{
					string label = "";
					for (int i = 1; i < is_branch_inst.size(); i++)
					{
						if (is_branch_inst[i] == ':')
							break;
						label += is_branch_inst[i];
					}
					label_begins[label] = ind;
					labels[ind] = label;
				}
				ind++;
			}
		}
		else
		{
			cout << "Failed to open file: " << filename << endl;
		}
		file.close();

		file.open(filename);

		if (file.is_open())
		{

			string line;
			int index = 0;
			while (getline(file, line))
			{
				while (line[0] == 9)
					line.erase(0, 1);
				while (line[0] == 32)
					line.erase(0, 1);
				if (line.size() > 0)
				{
					Instruction instr;
					if (line[0] == '#' || line[0] == '@')
					{
						// # comment
						continue;
					}
					else if (line[0] == 'b')
					{
						string branch = "";
						int temp = 0;
						for (int i = 0; i < line.size(); i++)
						{
							if (line[i] == ' ')
							{
								temp = i + 1;
								break;
							}
							branch += line[i];
						}
						string label = "";
						for (int i = temp + 1; i < line.size(); i++)
						{
							if (line[i] == ' ')
								break;
							label += line[i];
						}

						instr.name = branch;
						instr.operand = label_begins[label];
						input[index] = instr;
					}

					else if (line[0] == '.')
					{

						string label = "";
						for (int i = 1; i < line.size(); i++)
						{
							if (line[i] == ':')
								break;
							label += line[i];
						}
						// label_begins[label] = index;
						instr.name = "label";
						instr.operand = label_begins[label];
						input[index] = instr;
					}

					else
					{

						string operation = "";
						int temp = 0;
						for (int i = 0; i < line.size(); i++)
						{
							if (line[i] == ' ')
							{
								temp = i + 1;
								break;
							}
							operation += line[i];
						}
						string operand = "";
						int operant_integer_casted = 0;
						if (operation == "push")
						{
							for (int i = temp; i < line.size(); i++)
							{
								operant_integer_casted = operant_integer_casted * 10 + (line[i] - '0');
							}
							instr.operand = operant_integer_casted;
						}
						else if (operation == "call")
						{
							// call .foo
							for (int i = temp + 1; i < line.size(); i++)
							{
								operand += line[i];
							}
							instr.operand = label_begins[operand];
						}
						else if (operation == "var" || operation == "store" || operation == "load")
						{

							string var_name = "";
							for (int i = temp; i < line.size(); i++)
							{
								if (line[i] == ' ')
								{
									temp = i + 1;
									break;
								}
								var_name += line[i];
							}
							int var_name_hash = hashString(var_name);
							instr.operand = var_name_hash;
						}
						else
						{
							instr.operand = 0;
						}
						instr.name = operation;
						input[index] = instr;
					}

					index++;
				}
			}
			file.close();
		}
		else
		{
			cout << "Failed to open file: " << filename << endl;
		}

		// copy input to memory
		for (int i = 0; i < input.size(); i++)
		{
			mem[i] = input[i];
		}
	}

	void save_output(const int pc, const int gt, const int eq, const string branch, const vector<int> &dstack, const vector<int> &rstack, const string output, bool save_mem)
	{

		ofstream outputFile("../visualisation-sdl/output.txt", ios::app);
		if (save_mem)
		{
			ofstream clearFile("../visualisation-sdl/output.txt", ios::out);
			clearFile.close();
			outputFile << "Memory: ";
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i].name == "var" || input[i].name == "store" || input[i].name == "load")
				{
					string operandStr = to_string(input[i].operand);
					if (operandStr.length() > 4)
					{
						operandStr = operandStr.substr(0, 4);
					}
					outputFile << input[i].name << " " << operandStr << ",";
				}
				else if (input[i].name == "push")
				{
					outputFile << input[i].name << " " << input[i].operand << ",";
				}
				else if (input[i].name == "label")
					outputFile << "label " << labels[input[i].operand] << ",";
				else if (input[i].name == "b" || input[i].name == "beq" || input[i].name == "bgt" || input[i].name == "call")
					outputFile << input[i].name << " " << labels[input[i].operand] << ",";
				else
					outputFile << input[i].name << ",";
			}
			outputFile << endl;
		}
		else
		{
			Instruction instr = input[pc];
			outputFile << "\nCurrent Instruction: " << instr.name << " ";
			if (instr.name == "push")
			{
				outputFile << instr.operand << "\n";
			}
			else if (instr.name == "bgt" || instr.name == "beq" || instr.name == "b" || instr.name == "call")
			{
				outputFile << labels[instr.operand] << "\n";
			}
			else if (instr.name == "label")
			{
				outputFile << labels[pc] << "\n";
			}
			else
			{
				outputFile << "\n";
			}
			outputFile << "Data Stack: ";
			for (int i : dstack)
			{
				outputFile << i << " ";
			}
			outputFile << endl;
			outputFile << "Return Stack: ";
			for (int i : rstack)
			{
				outputFile << i << " ";
			}
			outputFile << endl;
			outputFile << "PC: " << pc << endl;
			outputFile << "GT: " << gt << endl;
			outputFile << "EQ: " << eq << endl;
			outputFile << "Branch: " << branch << endl;
			outputFile << "Output: " << output << endl;
		}
	}
};

#endif