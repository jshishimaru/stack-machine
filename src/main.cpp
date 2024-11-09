#include "machine.hpp"

int main(int argc, char* argv[]){

	if(argc != 2){
		cout << "Usage: ./a.out <input_file>" << endl;
		return 1;
	}
	Machine machine(1024);
	machine.ioModule.readInput(argv[1]);
	machine.run();

	return 0;
}
