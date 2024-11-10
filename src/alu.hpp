#ifndef ALU_HPP
#define ALU_HPP

#include <bits/stdc++.h>
#include "io.hpp"
using namespace std;

class ALU{

	public:

	vector <int>* dataStack;
	vector <int>* returnStack;
	vector <Instruction>* memory;
	map< int16_t , int> variables; // map of string to offsets
	int stack_pointer;
	int* pc;
	bool gt;
	bool eq;
	bool branch;

	ALU(){
		gt=0;
		eq=0;
		branch=0;
	}

	void setStacks( vector <int> &dStack , vector <int> &rStack, int& p_c , vector <Instruction> &memory){
		this->dataStack = &dStack;
		this->returnStack = &rStack;
		this->pc = &p_c;
		this->memory = &memory;
		gt=0;
		eq=0;
		branch=0;
		stack_pointer = 512;
		
	}

	int push( int operand ){
		dataStack->push_back(operand);
		// cout<<"datastack top : "<<dataStack->back()<<endl;
		return 0;	
	}

	int pop(){
		if (dataStack->size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		dataStack->pop_back();
		return 0;
	}

	int pushr(){
		if (dataStack->size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		returnStack->push_back(dataStack->back());
		dataStack->pop_back();
		return 0;
	}

	int drop(){
		if (dataStack->size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		dataStack->pop_back();
		return 0;
	}

	int dup(){
		
		if (dataStack->size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		dataStack->push_back(dataStack->back());
		return 0;
	}

	int over(){

		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		dataStack->push_back(*(dataStack->end()-2));
		return 0;
	}

	int swap(){

		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a);
		dataStack->push_back(b);
		return 0;

	}

	int sub()
	{
		if (dataStack->size() < 2){
			cout<<"stack khali hai bhaii...sub"<<endl;
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a - b);
		// cout<<"datastack top : "<<dataStack->back()<<endl;
		return 0;
	}

	int add(){
		if (dataStack->size() < 2) {
			cout<<"stack khali hai bhaii...add on pc: "<<(*pc)<<endl;
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a + b);
		// cout<<"datastack top : "<<dataStack->back()<<endl;
		return 0;
	}

	int inc(){
		if (dataStack->size() == 0) {
			cout<<"stack khali hai bhaii...inc"<<endl;
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a + 1);
		// cout<<"datastack top : "<<dataStack->back()<<endl;
		return 0;
	}

	int dec(){
		if (dataStack->size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a - 1);
		// cout<<"datastack top : "<<dataStack->back()<<endl;
		return 0;
	}

	int mul(){
		if (dataStack->size() < 2) {
			cout<<"stack khali hai bhaii...mul"<<endl;
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a * b);
		// cout<<"datastack top : "<<dataStack->back()<<endl;
		return 0;
	}

	int div(){
		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a / b);
		return 0;
	}

	int mod(){
		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a % b);
		return 0;
	}

	int bit_and(){
		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a & b);
		return 0;
	}

	int bit_or(){
		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a | b);
		return 0;
	}

	int bit_xor(){
		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(a ^ b);
		return 0;
	}

	int bit_not(){
		if (dataStack->size() == 0) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		dataStack->push_back(~a);
		return 0;
	}

	int cmp(){
		if (dataStack->size() < 2) {
			throw runtime_error("Stack underflow error");
		}
		int a = dataStack->back();
		dataStack->pop_back();
		int b = dataStack->back();
		dataStack->push_back(a);
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
			*pc = operand;
			// cout<<"jumped"<<endl;
		}
		return 0;
	}

	int eq_jump( int opr ){
		if( eq ){
			*pc = opr;
		}
		return 0;
	}

	int jump( int operand ){
		*pc = operand;
		return 0;
	}

	int ret(){
		if (returnStack->size() == 0) {
			cout<<"stack khali hai bhaii...ret"<<endl;
			throw runtime_error("Stack underflow error");
		}
		*pc = returnStack->back();
		returnStack->pop_back();
		return 0;
	}

	int call( int operand ){
		returnStack->push_back((*pc));
		*pc = operand;
		return 0;
	}

	int print(){
		cout << dataStack->back() << endl;
		return 0;
	}

	int halt(){
		(*pc) = -1;
		return 0;
	}

	int var( int var ){	
		int16_t var_name = var & 0xFFFF0000;
		int16_t val = var & 0x0000FFFF;
		variables[var_name] = stack_pointer;
		stack_pointer++;
		return 0;
	}

	int store_var( int var ){

		int16_t var_name = var & 0xFFFF0000;
		int16_t val = var & 0x0000FFFF;
		if( variables.find(var_name) == variables.end() ){
			throw runtime_error("Variable not declared");
		}
		Instruction variable = { "", val };
		(*memory)[variables[var_name]] = variable;
		return 0;
	}

	int load_var( int var ){

		int16_t var_name = var & 0xFFFF0000;
		int16_t val = var & 0x0000FFFF;
		if( variables.find(var_name) == variables.end() ){
			throw runtime_error("Variable not declared");
		}
		dataStack->push_back((*memory)[variables[var_name]].operand);
		return 0;
	}	


};


#endif