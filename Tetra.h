#ifndef Tetra_h
#define Tetra_h

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "SDL/SDL_mixer.h"

const int width = 800;
const int height = 600;

using namespace std;

class computerplayer
{
	vector <int> deck;
	vector <int> hand;
public:
	computerplayer();
	int displayDeck();
	pair<int,int> makeMove(SDL_Surface *screen); //Makes a move.  Behavior of function depends on
	//the value of private member variable human.
	friend class board;

private:
	int human, totalCards; // 0 == computer ;; 1 == human
	string name;


};

class humanplayer
{
	vector <int> deck;
	vector <int> hand;
public:
	humanplayer();
	//~computerplayer();
	int displayDeck();
	pair<int,int> makeMove(SDL_Surface *screen); //Makes a move.  Behavior of function depends on
	//the value of private member variable human.
	friend class board;

private:
	int human, totalCards; // 0 == computer ;; 1 == human
	string name;


};

class card
{
public:
	card(string name, int top, int right, int bottom, int left, int level, string imgFile);
	~card();
	int attack(card opponent);
	string printInfo()
	{
		char *line = (char*)malloc(sizeof(char) * 500);
		sprintf_s(line, sizeof(line), "%s\t%i\t%i\t%i\t%i\t%i\n", name.c_str(), top, right, bottom, left, level);
		return line;
	}
	SDL_Surface *img;
	int owner;


	friend class cardList;
	friend class board;

private:
	int left, right, top, bottom; // Attack powers for each direction
	int level; 
	int position; // On board
	string name, file;
};

class cardList
{
public:
	cardList();
	~cardList();
	int perror();
	vector<card> list;
	SDL_Surface *back;

private:
	int flag;
};

class board
{
public:
	board();
	~board();
	int mainMenu();
	void drawBoard();
	int update() { SDL_framerateDelay(fps); return SDL_Flip(screen); }
	int placeCard(int square, int player, int selectedCard);
	int checkArea(int x, int y);
	pair<int,int> findXY(int square);
	int makeMove(int player);
	void dispWinner();
	void applyText(string, int, int);
	int initPlayers();
	cardList *master;
	FPSmanager *fps;
	humanplayer p2;
	computerplayer p1;

private:
	void attack(int square, int player);
	void delay(int sec);
	int checkQuit(SDL_Event event);
	void drawMainMenu(int pos);

	int p1Score, p2Score;
	int selected;
	int alt;
	const static unsigned int maxLen = 8;
	pair< pair<int,int>, int> active[9]; // First two are original and current owner, third is card #
	SDL_Surface *screen;
	SDL_Surface *back;
	SDL_Surface *boardImg;
	SDL_Surface *arrow, *lArrow, *rArrow;
	Mix_Music *music;
	TTF_Font *font;
	TTF_Font *textFont;
	SDL_Color fontColor;


};

// SDL Functions begin here
int initSDL();
void cleanupSDL();
SDL_Surface* loadImage(string filename);
void drawBoard(SDL_Surface *screen);
#endif
