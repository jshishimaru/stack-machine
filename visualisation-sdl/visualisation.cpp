#include <iostream>
#include <vector>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <sstream>

using namespace std;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

vector <string> returnStack;
vector <string> dataStack;
vector <string> memory;
vector <string> currentMemory;
int pc;
int cur_inst;

string currentInstruction;

vector <vector <string> > inputBuffer(0,vector<string>(7));

void takeInput(){

	freopen( "output.txt", "r", stdin );

	string line;

	if( getline ( cin , line )){

		line = line.substr(8);

		stringstream ss(line);
		string item;
		while (getline(ss, item, ',')) {
			if( item!="" )memory.push_back(item);
		}	

	}

	while( getline(cin,line)){

		vector <string> instruction(7);
		if (line.find("Current Instruction:") != string::npos)
		{
			instruction[0] = line.substr(21);
			getline( cin , line );
			instruction[1] = line.substr(12);
			getline( cin , line );
			instruction[2] = line.substr(14);
			getline( cin , line );
			instruction[3] = line.substr(4);
			getline( cin , line );
			instruction[4] = line.substr(4);
			getline( cin , line );
			instruction[5] = line.substr(4);
			getline( cin , line );
			instruction[6] = line.substr(8);
			inputBuffer.push_back(instruction);
			instruction.clear();
		}

	}

	currentMemory.clear();
	int start = max(0, pc - 9);
	for (int i = start; i <=10 && i < memory.size(); ++i)
	{
		currentMemory.push_back(memory[i]);
	}
}

void init(){

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	font = TTF_OpenFont("fonts/font.ttf", 24);
	if (!font){
		cerr << "Failed to load font: " << TTF_GetError() << endl;
		exit(1);
	}

	window = SDL_CreateWindow("Visualisation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 900, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

}

bool isMouseOverButton(int mouseX, int mouseY, SDL_Rect buttonRect)
{
	return (mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w &&
			mouseY > buttonRect.y && mouseY < buttonRect.y + buttonRect.h);
}

void renderText(const string &text, int x, int y , int colorCode )
{
	SDL_Color color = {170, 20, 240, 255}; 
	if( colorCode == 1 ) color = {238, 238, 238, 255};
	if( colorCode == 2 ) color = {255, 101, 0, 255};
	if( colorCode == 3 ) color = {0, 0, 0, 255};
	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect destRect = {x, y, surface->w, surface->h};
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void updateStacks(int index){

	index = index % inputBuffer.size();
	currentInstruction = inputBuffer[index][0];
	pc = stoi(inputBuffer[index][3]);

	string temp = inputBuffer[index][1];
	dataStack.clear();
	stringstream ss(temp);
	string item;
	while (getline(ss, item, ' ')) {
		if( item!="" )dataStack.push_back(item);
	}
	temp = inputBuffer[index][2];
	returnStack.clear();
	stringstream ss1(temp);
	while (getline(ss1, item, ' ')) {
		if( item!="" )returnStack.push_back(item);
	}

	currentMemory.clear();
	int start = max(0, pc - 9);
	for (int i = start; i <= (10+start) && i < memory.size(); ++i)
	{
		currentMemory.push_back(memory[i]);
	}

	if( pc < 9) cur_inst = pc;
	else cur_inst = 9;

}

int main ( int argc, char* argv[] ){

	init();
	takeInput();

	bool running = true ;
	int instructionIndex = 0;
	SDL_Rect nextButtonRect = {600, 750, 150, 50};
	SDL_Rect prevButtonRect = {400, 750, 150, 50};
	SDL_Rect instructionTextBox = {250, 600, 600, 50};

	SDL_Rect dataStackRect = {10, 30, 300, 500};
	SDL_Rect returnStackRect = {360 , 30, 300, 500};

	while( running ){

		SDL_Event event;
		while( SDL_PollEvent(&event) ){
			if( event.type == SDL_QUIT ){
				running = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				if (isMouseOverButton(mouseX, mouseY, nextButtonRect))
				{
					instructionIndex++;
					updateStacks(instructionIndex);
				}
				else if (isMouseOverButton(mouseX, mouseY, prevButtonRect))
				{
					instructionIndex--;
					if (instructionIndex < 0)
						instructionIndex = inputBuffer.size() - 1;
					updateStacks(instructionIndex);
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
		SDL_RenderFillRect(renderer, &nextButtonRect);
		renderText("Next", nextButtonRect.x + 20, nextButtonRect.y + 10 , 3);

		SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
		SDL_RenderFillRect(renderer, &prevButtonRect);
		renderText("Previous", prevButtonRect.x + 10, prevButtonRect.y + 10 , 3 );

		SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
		SDL_RenderFillRect(renderer, &instructionTextBox);
		renderText("Current Instruction: " + currentInstruction, instructionTextBox.x + 10, instructionTextBox.y + 10, 3);
		renderText("                     " + currentInstruction, instructionTextBox.x + 10, instructionTextBox.y + 10, 0);
		//		renderText( currentInstruction , instructionTextBox.x + 300 , instructionTextBox.y + 10 , 3);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for boundaries
		SDL_RenderDrawRect(renderer, &dataStackRect);
		SDL_RenderDrawRect(renderer, &returnStackRect);

		renderText("Data Stack:", 50, 50 , 2);
		for (size_t i = 0; i < dataStack.size(); ++i)
		{
			renderText(dataStack[i], 120, 100 + i * 30 , 0);
		}

		renderText("Return Stack:", 400, 50 , 2);
		for (size_t i = 0; i < returnStack.size(); ++i)
		{	
			renderText(returnStack[i], 420, 100 + i * 30 , 0);
		}

		renderText("Memory Stack:", 950, 50 , 2);
		for (size_t i = 0; i < currentMemory.size(); ++i)
		{
			if( i == cur_inst )
			{ 
				renderText( currentMemory[i], 950 , 100 + i * 35 , 1);
				renderText( "PC :" , 880 , 100 + i * 35 , 1);
			}
			else renderText(currentMemory[i], 950, 100 + i * 35 , 0);
		}

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;

}
