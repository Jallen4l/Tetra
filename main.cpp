#include "Tetra.h"
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_ttf.h>

int main(int argc, char **argv)
{
	srand( (unsigned int)time(NULL));
	initSDL();

	board *gameBoard = new board;
	if(gameBoard->master->perror() != 0)
		return 1;


	int load = gameBoard->mainMenu();
	if(load == -1)
		return 0;

	gameBoard->initPlayers();
	gameBoard->update();
	gameBoard->drawBoard();
	//gameBoard->selectHands();
	int first, second, turn = rand() % 2;

	if(turn == 0) { first = 1; second = 2; }
	else if(turn == 1) { first = 2; second = 1; }

	gameBoard->makeMove(first);
	gameBoard->update();
	for(int i = 0; i < 4; i++)
		if(gameBoard->makeMove(second) == -1 || gameBoard->makeMove(first) == -1)
			return 1;

	gameBoard->dispWinner();

	return 0;
}
