#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <vector>
#include <string>
#include "alu.hpp"
#include "io.hpp"
using namespace std;

struct Instruction{
	string name;
	int operand;
};

class Machine{

	public:
	int memory_size;
	vector <Instruction> memory;
	vector <int> dataStack;
	vector <int> returnStack;
	ALU alu;
	IO ioModule;
	int pc;

	Machine( int memory_size ){
		this->memory_size = memory_size;
		this->memory = vector<Instruction>(memory_size);
		this->dataStack = vector<int>();
		this->returnStack = vector<int>();
		this->pc = 0;
	}



};


#endif