#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <bits/stdc++.h>
#include "alu.hpp"
#include "io.hpp"
using namespace std;

class Machine
{

public:
	int memory_size;
	vector<Instruction> memory;
	vector<int> dataStack;
	vector<int> returnStack;
	ALU alu;
	IO ioModule;
	int pc;

	Machine(int memory_size, string filename)
	{
		this->memory_size = memory_size;
		this->memory.resize(memory_size);
		this->dataStack = vector<int>();
		this->returnStack = vector<int>();
		ioModule.readInput(filename, (this->memory));
		this->alu.setStacks(this->dataStack, this->returnStack, this->pc, this->memory);
		this->pc = ioModule.label_begins["main"];
	}

	void run()
	{

		// for( int i = 0 ; i < ioModule.labels.size() ; i++ )
		// {
		// 	cout<<ioModule.labels[i]<<endl;
		// }

		ofstream outputFile("../visualisation-sdl/output.txt");
		cout << "starting the machine..." << endl;

		outputFile << "Memory: ";
		for (int i = 0; i < ioModule.input.size(); i++)
		{
			if (ioModule.input[i].name == "var" || ioModule.input[i].name == "store" || ioModule.input[i].name == "load")
			{
				string operandStr = to_string(ioModule.input[i].operand);
				if (operandStr.length() > 4)
				{
					operandStr = operandStr.substr(0, 4);
				}
				outputFile << ioModule.input[i].name << " " << operandStr << ",";
			}
			else if (ioModule.input[i].name == "push")
			{
				outputFile << ioModule.input[i].name << " " << ioModule.input[i].operand << ",";
			}
			else if (ioModule.input[i].name == "label")
				outputFile << "label " << ioModule.labels[ioModule.input[i].operand] << ",";
			else if (ioModule.input[i].name == "b" || ioModule.input[i].name == "beq" || ioModule.input[i].name == "bgt" || ioModule.input[i].name == "call")
				outputFile << ioModule.input[i].name << " " << ioModule.labels[ioModule.input[i].operand] << ",";
			else
				outputFile << ioModule.input[i].name << ",";
		}
		outputFile << endl;
		while (pc != -1 && pc < ioModule.input.size())
		{
			Instruction instr = ioModule.input[pc];
			int temp = pc;
			outputFile << "\nCurrent Instruction: " << instr.name << " ";
			if (instr.name == "push")
			{
				outputFile << instr.operand << "\n";
			}
			else if (instr.name == "bgt" || instr.name == "beq" || instr.name == "b" || instr.name == "call")
			{
				outputFile << ioModule.labels[instr.operand] << "\n";
			}
			else if (instr.name == "label")
			{
				outputFile << ioModule.labels[pc] << "\n";
			}
			else
			{
				outputFile << "\n";
			}

			outputFile << "Data Stack: ";
			for (int i : dataStack)
			{
				outputFile << i << " ";
			}
			outputFile << endl;
			outputFile << "Return Stack: ";
			for (int i : returnStack)
			{
				outputFile << i << " ";
			}
			outputFile << endl;
			outputFile << "PC: " << temp << endl;

			if (instr.name == "push")
			{
				alu.push(instr.operand);
			}
			else if (instr.name == "bgt")
			{
				alu.gt_jump(instr.operand);
			}
			else if (instr.name == "beq")
			{
				alu.eq_jump(instr.operand);
			}
			else if (instr.name == "b")
			{
				alu.jump(instr.operand);
			}
			else if (instr.name == "call")
			{
				alu.call(instr.operand);
			}
			else if (instr.name == "ret")
			{
				alu.ret();
			}
			else if (instr.name == "pop")
			{
				alu.pop();
			}
			else if (instr.name == "pushr")
			{
				alu.pushr();
			}
			else if (instr.name == "drop")
			{
				alu.drop();
			}
			else if (instr.name == "add")
			{
				alu.add();
			}
			else if (instr.name == "sub")
			{
				alu.sub();
			}
			else if (instr.name == "mul")
			{
				alu.mul();
			}
			else if (instr.name == "div")
			{
				alu.div();
			}
			else if (instr.name == "cmp")
			{
				alu.cmp();
			}
			else if (instr.name == "and")
			{
				alu.bit_and();
			}
			else if (instr.name == "or")
			{
				alu.bit_or();
			}
			else if (instr.name == "xor")
			{
				alu.bit_xor();
			}
			else if (instr.name == "not")
			{
				alu.bit_not();
			}
			else if (instr.name == "dup")
			{
				alu.dup();
			}
			else if (instr.name == "over")
			{
				alu.over();
			}
			else if (instr.name == "swap")
			{
				alu.swap();
			}
			else if (instr.name == "print")
			{
				alu.print();
			}
			else if (instr.name == "label")
			{
				// do nothing
			}
			else if (instr.name == "nop")
			{
				// do nothing
			}
			else if (instr.name == "inc")
			{
				alu.inc();
			}
			else if (instr.name == "dec")
			{
				alu.dec();
			}
			else if (instr.name == "halt")
			{
				alu.halt();
			}
			else if (instr.name == "var")
			{
				alu.var(instr.operand);
			}
			else if (instr.name == "store")
			{
				alu.store_var(instr.operand);
			}
			else if (instr.name == "load")
			{
				alu.load_var(instr.operand);
			}
			else if (instr.name == "halt")
			{
				alu.halt();
				return;
			}
			else
			{
				throw runtime_error("Invalid instruction");
			}

			if (instr.name != "halt" && instr.name != "ret")
			{
				pc++;
			}
		}
		cout << "Ending the machine..." << endl;
	}
};

#endif