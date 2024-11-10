#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <bits/stdc++.h>
#include "alu.hpp"
#include "io.hpp"
using namespace std;

class Machine{

	public:
	int memory_size;
	vector <Instruction> memory;
	vector <int> dataStack;
	vector <int> returnStack;
	ALU alu;
	IO ioModule;
	int pc;

	Machine( int memory_size , string filename ){
		this->memory_size = memory_size;
		this->memory.resize(memory_size);
		this->dataStack = vector<int>();
		this->returnStack = vector<int>();
		ioModule.readInput(filename,(this->memory));
		this->alu.setStacks(this->dataStack, this->returnStack,this->pc);
		this->pc = ioModule.label_begins["main"];
		
	}

	void run(){
		ofstream outputFile("output.txt");
		cout<<"starting the machine..."<<endl;
		
		while( pc!=-1 && pc < ioModule.input.size() ){
			outputFile << "Current Instruction: ";

			Instruction instr = ioModule.input[pc];
			if( instr.name == "push"){
				outputFile << instr.name << " " << instr.operand << "\n";
				alu.push(instr.operand);
			}
			else if( instr.name == "bgt"){
				alu.gt_jump(instr.operand);
			}
			else if( instr.name == "beq"){
				alu.eq_jump(instr.operand);
			}
			else if( instr.name == "b"){
				alu.jump(instr.operand);
			}
			else if( instr.name == "call"){
				alu.call(instr.operand);
			}
			else if ( instr.name == "ret"){
				alu.ret();
			}
			else if( instr.name == "pop"){
				alu.pop();
			}
			else if( instr.name == "pushr"){
				alu.pushr();
			}
			else if( instr.name == "drop"){
				alu.drop();
			}
			else if( instr.name == "add"){
				alu.add();
			}
			else if( instr.name == "sub"){
				alu.sub();
			}
			else if( instr.name == "mul"){
				alu.mul();
			}
			else if( instr.name == "div"){
				alu.div();
			}
			else if( instr.name == "cmp"){
				alu.cmp();
			}
			else if( instr.name == "and"){
				alu.bit_and();
			}
			else if( instr.name == "or"){
				alu.bit_or();
			}
			else if( instr.name == "xor"){
				alu.bit_xor();
			}
			else if( instr.name == "not"){
				alu.bit_not();
			}
			else if( instr.name == "dup"){
				alu.dup();
			}
			else if( instr.name == "over"){
				alu.over();
			}
			else if( instr.name == "swap"){
				alu.swap();
			}
			else if( instr.name == "print"){
				alu.print();
			}
			else if( instr.name == "label"){
				// do nothing
			}
			else if( instr.name == "nop"){
				// do nothing
			}
			else if( instr.name == "inc"){
				alu.inc();
			}
			else if( instr.name == "dec"){
				alu.dec();
			}
			else if( instr.name == "halt"){
				alu.halt();
				break;
			}
			else{
				throw runtime_error("Invalid instruction");
			}

			if(instr.name != "halt" || instr.name != "ret"){
				pc++;
			}
			else if(instr.name == "halt"){
				pc = -1;
			}

			// cout << "dataStack: ";
			// for(int i : dataStack){
			// 	cout << i << " ";
			// }
			// cout << endl;
			
		}
		cout<<"Ending the machine..."<<endl;
	}

};


#endif