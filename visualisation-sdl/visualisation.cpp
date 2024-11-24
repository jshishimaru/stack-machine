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
TTF_Font *font_40 = NULL;
TTF_Font *font_24 = NULL;
TTF_Font *font_64 = NULL;

vector<string> returnStack;
vector<string> dataStack;
vector<string> memory;
vector<string> currentMemory;
int pc;
int cur_inst;
int flagsE;
int flagsG;

string output;
string currentBranch;
string currentInstruction;

vector<vector<string>> inputBuffer(0, vector<string>(8));

void takeInput()
{

	freopen("output.txt", "r", stdin);

	string line;

	if (getline(cin, line))
	{

		line = line.substr(8);

		stringstream ss(line);
		string item;
		while (getline(ss, item, ','))
		{
			if (item != "")
				memory.push_back(item);
		}
	}

	while (getline(cin, line))
	{

		vector<string> instruction(8);
		if (line.find("Current Instruction:") != string::npos)
		{
			instruction[0] = line.substr(21);
			getline(cin, line);
			instruction[1] = line.substr(12);
			getline(cin, line);
			instruction[2] = line.substr(14);
			getline(cin, line);
			instruction[3] = line.substr(4);
			getline(cin, line);
			instruction[4] = line.substr(4);
			getline(cin, line);
			instruction[5] = line.substr(4);
			getline(cin, line);
			instruction[6] = line.substr(8);
			getline(cin, line);
			instruction[7] = line.substr(8);
			inputBuffer.push_back(instruction);
			instruction.clear();
		}
	}

	currentMemory.clear();
	int start = max(0, pc - 9);
	for (int i = start; i <= 10 && i < memory.size(); ++i)
	{
		currentMemory.push_back(memory[i]);
	}
}

void init()
{

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	font_24 = TTF_OpenFont("fonts/font-bold.ttf", 14);
	font_40 = TTF_OpenFont("fonts/font.ttf", 40);
	font_64 = TTF_OpenFont("fonts/font.ttf", 64);
	if (!font_24 || !font_40 || !font_64)
	{
		cerr << "Failed to load font: " << TTF_GetError() << endl;
		exit(1);
	}

	window = SDL_CreateWindow("Visualisation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 680, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

bool isMouseOverButton(int mouseX, int mouseY, SDL_Rect buttonRect)
{
	return (mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w &&
			mouseY > buttonRect.y && mouseY < buttonRect.y + buttonRect.h);
}

void renderText(const string &text, int x, int y, int colorCode, int fontCode)
{
	SDL_Color color = {170, 20, 240, 255}; // default color
	if (colorCode == 1)
		color = {238, 238, 238, 255}; // white
	if (colorCode == 2)
		color = {255, 101, 0, 255}; // pink
	if (colorCode == 3)
		color = {0, 0, 0, 255}; // black
	if (colorCode == 4)
		color = {121, 203, 96, 255}; // green
	if (colorCode == 5)
		color = {251, 234, 52, 255};

	TTF_Font *font = font_24;
	if (fontCode == 1)
		font = font_40;
	if (fontCode == 2)
		font = font_64;

	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect destRect = {x, y, surface->w, surface->h};
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void updateStacks(int index)
{

	index = index % inputBuffer.size();
	currentInstruction = inputBuffer[index][0];
	pc = stoi(inputBuffer[index][3]);

	string temp = inputBuffer[index][1];
	dataStack.clear();
	stringstream ss(temp);
	string item;
	while (getline(ss, item, ' '))
	{
		if (item != "")
			dataStack.push_back(item);
	}
	temp = inputBuffer[index][2];
	returnStack.clear();
	stringstream ss1(temp);
	while (getline(ss1, item, ' '))
	{
		if (item != "")
			returnStack.push_back(item);
	}

	currentMemory.clear();
	int start = max(0, pc - 9);
	for (int i = start; i <= (10 + start) && i < memory.size(); ++i)
	{
		currentMemory.push_back(memory[i]);
	}

	if (pc < 9)
		cur_inst = pc;
	else
		cur_inst = 9;

	currentBranch = inputBuffer[index][6];
	flagsE = stoi(inputBuffer[index][5]);
	flagsG = stoi(inputBuffer[index][4]);
	output = inputBuffer[index][7];
}

int main(int argc, char *argv[])
{

	init();
	takeInput();

	bool running = true;
	int instructionIndex = 0;
	SDL_Rect nextButtonRect = {417, 483, 101, 51};
	SDL_Rect prevButtonRect = {303, 483, 101, 51};
	SDL_Rect instructionTextBox = {350, 600, 600, 50};

	SDL_Rect dataStackRect = {37, 91, 114, 443};
	SDL_Rect dataStack2Rect = {37, 91, 114, 43};
	SDL_Rect returnStackRect = {167, 91, 114, 43};
	SDL_Rect returnStack2Rect = {167, 91, 114, 443};
	SDL_Rect memoryStackRect = {539, 91, 240, 443};
	SDL_Rect memoryStack2Rect = {539, 91, 240, 46};
	SDL_Rect currentInstructionRect = {37, 553, 596, 44};
	SDL_Rect currentBranchRect = {37, 612, 596, 44};
	SDL_Rect centerRect = {303, 263, 215, 205};
	SDL_Rect FlagsERect = {319, 283, 182, 44};
	SDL_Rect FlagsGRect = {319, 343, 182, 44};
	SDL_Rect outputRect = {319, 403, 182, 44};
	SDL_Rect titleRect = {32, 16, 742, 65};

	while (running)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
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
		SDL_RenderDrawRect(renderer, &nextButtonRect);
		renderText("NEXT", nextButtonRect.x + 20, nextButtonRect.y + 4, 1, 1);

		SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
		SDL_RenderDrawRect(renderer, &prevButtonRect);
		renderText("PREV", prevButtonRect.x + 20, prevButtonRect.y + 4, 1, 1);

		SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
		renderText("CURRENT INSTRUCTION: ", currentInstructionRect.x + 10, currentInstructionRect.y, 4, 1);
		renderText("                     " + currentInstruction, currentInstructionRect.x + 130, currentInstructionRect.y, 0, 1);

		renderText("CURRENT BRANCH: ", currentBranchRect.x + 10, currentBranchRect.y, 4, 1);
		renderText("                " + currentBranch, currentBranchRect.x + 110, currentBranchRect.y, 0, 1);

		renderText("STACK MACHINE VISUALISER", titleRect.x + 88, titleRect.y - 4, 1, 2);
		renderText("OUTPUT:", outputRect.x + 10, outputRect.y, 4, 1);
		renderText("FLAGS.E:", FlagsERect.x + 10, FlagsERect.y, 4, 1);
		renderText("FLAGS.GT:", FlagsGRect.x + 10, FlagsGRect.y, 4, 1);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for boundaries
		SDL_RenderDrawRect(renderer, &dataStackRect);
		SDL_RenderDrawRect(renderer, &dataStack2Rect);
		SDL_RenderDrawRect(renderer, &returnStackRect);
		SDL_RenderDrawRect(renderer, &returnStack2Rect);
		SDL_RenderDrawRect(renderer, &memoryStackRect);
		SDL_RenderDrawRect(renderer, &memoryStack2Rect);
		SDL_RenderDrawRect(renderer, &currentInstructionRect);
		SDL_RenderDrawRect(renderer, &currentBranchRect);
		SDL_RenderDrawRect(renderer, &centerRect);
		SDL_RenderDrawRect(renderer, &FlagsERect);
		SDL_RenderDrawRect(renderer, &FlagsGRect);
		SDL_RenderDrawRect(renderer, &outputRect);
		SDL_RenderDrawRect(renderer, &titleRect);

		renderText("DATA", dataStack2Rect.x + 23, dataStack2Rect.y, 4, 1);
		for (size_t i = 0; i < dataStack.size(); ++i)
		{
			renderText(dataStack[i], dataStackRect.x + 50, dataStackRect.y + 50 + i * 35, 0, 1);
		}

		renderText("RETURN", returnStack2Rect.x + 10, returnStack2Rect.y, 4, 1);
		for (size_t i = 0; i < returnStack.size(); ++i)
		{
			renderText(returnStack[i], returnStackRect.x + 50, returnStackRect.y + 50 + i * 35, 0, 1);
		}

		renderText("MEMORY", memoryStack2Rect.x + 75, memoryStack2Rect.y + 2, 5, 1);
		for (size_t i = 0; i < currentMemory.size(); ++i)
		{
			if (i == cur_inst)
			{
				renderText(currentMemory[i], memoryStackRect.x + 20, memoryStackRect.y + 60 + i * 35, 1, 0);
				renderText("<- PC", memoryStackRect.x + 180, memoryStackRect.y + 60 + i * 35, 1, 0);
			}
			else
				renderText(currentMemory[i], memoryStackRect.x + 20, memoryStackRect.y + 60 + i * 35, 0, 0);
		}

		renderText(to_string(flagsE), FlagsERect.x + 140, FlagsERect.y, 0, 1);
		renderText(to_string(flagsG), FlagsGRect.x + 140, FlagsGRect.y, 0, 1);
		renderText( "         " + output , outputRect.x + 40, outputRect.y, 0, 1);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font_64);
	TTF_CloseFont(font_24);
	TTF_CloseFont(font_40);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
