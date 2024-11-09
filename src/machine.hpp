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
	vector <int> memory;
	vector <int> dataStack;
	vector <int> returnStack;
	ALU alu;
	IO ioModule;
	int pc;

	Machine( int memory_size ){
		this->memory_size = memory_size;
		this->memory = vector<int>(memory_size, 0);
		this->dataStack = vector<int>(1024,0);
		this->returnStack = vector<int>(1024,0);
		this->pc = 0;
	}



};


#endif