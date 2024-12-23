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
	string current_branch = "main";
	bool isPrint = false;
	string output = " ";

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

		cout << "starting the machine..." << endl;
		ioModule.save_output(pc, alu.gt, alu.eq, current_branch, dataStack, returnStack, output, true);

		while (pc != -1 && pc < ioModule.input.size())
		{
			Instruction instr = ioModule.input[pc];
			int temp = pc;
			ioModule.save_output(pc, alu.gt, alu.eq, current_branch, dataStack, returnStack, output, false);

			if (instr.name == "push")
			{
				alu.push(instr.operand);
			}
			else if (instr.name == "bgt")
			{
				if (alu.gt == 1)
				{
					current_branch = ioModule.labels[instr.operand];
				}
				alu.gt_jump(instr.operand);
			}
			else if (instr.name == "beq")
			{
				if (alu.eq == 1)
				{
					current_branch = ioModule.labels[instr.operand];
				}
				alu.eq_jump(instr.operand);
			}
			else if (instr.name == "b")
			{
				current_branch = ioModule.labels[instr.operand];
				alu.jump(instr.operand);
			}
			else if (instr.name == "call")
			{
				current_branch = ioModule.labels[instr.operand];
				alu.call(instr.operand);
			}
			else if (instr.name == "ret")
			{
				current_branch = ioModule.labels[ioModule.input[alu.returnStack->back()].operand];
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
				output = to_string(alu.dataStack->back());
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