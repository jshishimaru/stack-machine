#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
using namespace std;

struct Instruction{
	string name;
	int operand;
};

class IO{
	public:
	vector <Instruction> input;
	vector <int> output;
	map <string, int> label_begins;
	vector <Instruction>* memory;
	vector <int>* dataStack;

	IO(){
		this->input = vector<Instruction>(1024);
		this->output = vector<int>(1024,0);
		this->label_begins = map<string, int>();
	}

	void setMemory(vector <Instruction>& memory , vector <int>& dataStack){
		this->memory = &memory;
		this->dataStack = &dataStack;
	}

	void print(){
		cout << *(dataStack->end()) << endl;
	}
	
	void readInput(string filename){
		ifstream file(filename);
		if(file.is_open()){
			string line;
			
			int index = 0;
			string current_label;
			while(getline(file, line)){
				while(line[0]==' ') line.erase(0,1);
				if(line.size()>0){
					Instruction instr;
					if(line[0] == 'b'){

						// b .foo
						// beq .foo
						// bgt .foo
						// instruction -> { b/beq/bgt , label_index }

						string branch = "";
						int temp = 0;
						for (int i = 0; i < line.size(); i++){
							if(line[i] == ' '){
								temp = i+1;
								break;
							}
							branch += line[i];
						}
						string label = "";
						for (int i = temp+1; i < line.size(); i++){
							if(line[i] == ' ') break;
							label += line[i];
						}
						current_label = label;

						instr.name = branch;
						instr.operand = label_begins[label];
						input[index] = instr;
					}

					else if(line[0] == '.'){

						// .foo:
						// instruction -> {label, 0}

						string label = "";
						for (int i = 1; i < line.size(); i++){
							if(line[i]==':') break;
							label += line[i];
						}
						label_begins[label] = index;
						instr.name = "label";
						instr.operand = 0;
						input[index] = instr;
						
					}

					else{
						// add 
						// sub 
						// mul 
						// div 
						// pop
						// push 5

						// instruction -> { add/sub/mul/div/pop , operand }
						// instruction -> { push , 5 }

						string operation = "";
						int temp = 0;
						for (int i = 0; i < line.size(); i++){
							if(line[i] == ' '){
								temp = i+1;
								break;
							}
							operation += line[i];
						}
						string operand = "";
						if(operation == "push"){
							for (int i = temp; i < line.size(); i++){
								operand += line[i];
							}
							instr.operand = stoi(operand);
						}
						else{
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
		else{
			cout << "Failed to open file: " << filename << endl;
		}
	}

	

};

#endif