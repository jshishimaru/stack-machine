#include "machine.hpp"
#include <bits/stdc++.h>

int main(int argc, char* argv[]){

	if(argc != 2){
		cout << "Usage: ./a.out <input_file>" << endl;
		return 1;
	}
	Machine machine(1024,argv[1]);
	for (int i = 0; i < 20; i++){
        // if(machine.ioModule.input[i].name == "halt") break;
        cout<<machine.ioModule.input[i].name<<" "<<machine.ioModule.input[i].operand<<endl;
    }
    cout<<endl;
    machine.run();

	return 0;
}
