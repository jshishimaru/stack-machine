#ifndef ALU_HPP
#define ALU_HPP

#include <vector>
#include <stdexcept>
#include <iostream>
using namespace std;

class ALU{

	public:

	vector <int> dataStack;
	vector <int> returnStack;
	int pc;
	bool gt;
	bool eq;

	void setStacks( vector <int> &dataStack, vector <int> &returnStack, int &pc ){
		this->dataStack = dataStack;
		this->returnStack = returnStack;
		this->pc = pc;
		gt=0;
		eq=0;
	}

	int push( int operand ){
		dataStack.push_back(operand);
		return 0;	
	}

	int pop(){
		if (dataStack.size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		dataStack.pop_back();
		return 0;
	}

	int pushr(){
		if (dataStack.size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		returnStack.push_back(dataStack.back());
		dataStack.pop_back();
		return 0;
	}

	int drop(){
		if (returnStack.size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		dataStack.pop_back();
		return 0;
	}

	int dup(){
		
		if (dataStack.size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		dataStack.push_back(dataStack.back());
		return 0;
	}

	int over(){

		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		dataStack.push_back(dataStack[dataStack.size()-2]);
		return 0;
	}

	int swap(){

		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a);
		dataStack.push_back(b);
		return 0;

	}

	int sub()
	{
		if (dataStack.size() < 2)
		{
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a - b);
		return 0;
	}

	int add(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a + b);
		return 0;
	}

	int mul(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a * b);
		return 0;
	}

	int div(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a / b);
		return 0;
	}

	int mod(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a % b);
		return 0;
	}

	int bit_and(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a & b);
		return 0;
	}

	int bit_or(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a | b);
		return 0;
	}

	int bit_xor(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		dataStack.pop_back();
		int b = dataStack.back();
		dataStack.pop_back();
		dataStack.push_back(a ^ b);
		return 0;
	}

	int cmp(){
		if (dataStack.size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack.back();
		int b = dataStack[dataStack.size()-2];
		if( a > b ){
			gt=1;
			eq=0;
		}
		else if( a == b ){
			gt=0;
			eq=1;
		}
		else{
			gt=0;
			eq=0;
		}
		return 0;
    };

	int gt_jump( int operand ){
		if( gt ){
			pc = operand;
		}
		return 0;
	}

	int eq_jump( int operand ){
		if( eq ){
			pc = operand;
		}
		return 0;
	}

	int jump( int operand ){
		pc = operand;
		return 0;
	}

	int ret(){
		if (returnStack.size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		pc = returnStack.back();
		returnStack.pop_back();
		return 0;
	}

	int call( int operand ){
		returnStack.push_back(pc);
		pc = operand;
		return 0;
	}

	int halt(){
		pc = -1;
		return 0;
	}

	int print(){
		if (dataStack.size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		cout << dataStack.back() << endl;
		return 0;
	}

};


#endif