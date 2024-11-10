#ifndef IO_HPP
#define IO_HPP

#include <bits/stdc++.h>
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
	map <int,string> labels;
	vector <Instruction>* memory;
	vector <int>* dataStack;

	IO(){
		this->input = vector<Instruction>(1024);
		this->output = vector<int>(1024,0);
		this->label_begins = map<string, int>();
	}
	
	void readInput(string filename,vector <Instruction>& mem){
		ifstream file(filename);
		if(file.is_open()){
			string is_branch_inst;
			int ind = 0;
			while(getline(file, is_branch_inst)){
				while(is_branch_inst[0]==9) is_branch_inst.erase(0,1);
				while(is_branch_inst[0]==32) is_branch_inst.erase(0,1);
				if(is_branch_inst.size()==0) continue;
				if(is_branch_inst[0] == '.'){

					// .foo:

					string label = "";
					for (int i = 1; i < is_branch_inst.size(); i++){
						if(is_branch_inst[i]==':') break;
						label += is_branch_inst[i];
					}
					label_begins[label] = ind;
					labels[ind] = label;
					
				}
				ind++;
			}
		}
		else{
			cout << "Failed to open file: " << filename << endl;
		}
		file.close();

		file.open(filename);
		
		if(file.is_open()){
			
			string line;
			int index = 0;
			while(getline(file, line)){
				while(line[0]==9) line.erase(0,1);
				while(line[0]==32) line.erase(0,1);
				if(line.size()>0){
					Instruction instr;
					if(line[0]=='#' || line[0]=='@'){
						// # comment
						continue;
					}
					else if(line[0] == 'b'){

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
						// label_begins[label] = index;
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
						// call .foo

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
						int oprnd = 0;
						if(operation == "push"){
							for (int i = temp; i < line.size(); i++){
								oprnd = oprnd*10 + (line[i]-'0');
							}
							instr.operand = oprnd;
						}
						else if(operation == "call"){
							// call .foo
							for (int i = temp+1; i < line.size(); i++){
								operand += line[i];
							}
							instr.operand = label_begins[operand];

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

		// copy input to memory
		for (int i = 0; i < input.size(); i++){
			mem[i] = input[i];
		}


		// scripting output file
	
		ofstream outputFile("output.txt");
		if (outputFile.is_open()) {
			outputFile << "Memory: " << endl;
			for (int i = 0; i < input.size(); i++) {
				if(input[i].name == "push")
					outputFile << input[i].name << input[i].operand << ",";
				else if(input[i].name == "label")
					outputFile << "label "<<labels[input[i].operand] << ",";
				else if(input[i].name == "b" || input[i].name == "beq" || input[i].name == "bgt" || input[i].name == "call")
					outputFile << input[i].name << " " << labels[input[i].operand] << ",";
				else
					outputFile << input[i].name << ",";
			}
			outputFile.close();
		} else {
			cout << "Failed to open output file." << endl;
		}

	}
	

};

#endif