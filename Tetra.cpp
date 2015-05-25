#define _CRT_SECURE_NO_WARNINGS
#include "Tetra.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <utility>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "SDL/SDL_mixer.h"

//Constant variable storing filename of cards
const string filename = "cards.txt";
//Constant variables storing positions
const int cardX = 93, cardY = 120, cardOff = 65;
const int p1x = 665, p1y = 115;
const int p2x = 135 - cardX, p2y = 115;
const int xBoard = 150, yBoard = 100;
const int x_1 = 78 + xBoard, x_2 = 194 + xBoard, x_3 = 312 + xBoard;
const int y_1 = 38 + yBoard, y_2 = 179 + yBoard, y_3 = 328 + yBoard;
const int sWidth = 114, sHeight = 140;
const int xOff = 11;
const int yOff = 11;
const int red = 0xFF0000FF, blue = 0x0000FFFF;






humanplayer::humanplayer() {

	int j;
	human = 1;
	name = "Jack";
	totalCards = 5;
	//Loop to add a random card from the list to the hand 5 times
	for(j = 0; j < totalCards; j++)
		hand.push_back(rand() % 110);




}

computerplayer::computerplayer() {

	int j;
	human = 0;
	name = "Computer";
	totalCards = 5;
	//Loop to add a random card from the list to the hand 5 times
	for(j = 0; j < totalCards; j++)
		hand.push_back(rand() % 110);




}


//Initalise players
int board::initPlayers()
{

	humanplayer p2;
	computerplayer p1;

	return 0;
}


board::board()
{

	// Set video mode -- fullscreen optional
	screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE  /*| SDL_FULLSCREEN */);

	// Load all of the needed images except for the cards
	back = loadImage("img/background.png");
	boardImg = loadImage("img/board.png");
	arrow = loadImage("img/arrow.png");
	//music = Mix_LoadMUS("introv3.ogg");


	// Construct new cardList (includes loading more images)
	master = new cardList;
	//Set total cards as the amount of cards in cardList
	p1.totalCards = master->list.size();
	p2.totalCards = master->list.size();
	selected = -1;
	//Set default scores
	p1Score = p2Score = 5;

	// Prepare framerate managment structs
	fps = new FPSmanager;
	SDL_initFramerate(fps);
	SDL_setFramerate(fps, 60);

	// Prepare font structs
	textFont = font = TTF_OpenFont("fonts/OpenSans-Regular.TTF", 48);
	//Clear active board squares
	for(int i = 0; i < 9; i++)
		active[i].first.first = active[i].first.second = active[i].second = -1;

}

board::~board()
{
	//Set SDL surfaces
	SDL_FreeSurface(screen);
	SDL_FreeSurface(back);
	SDL_FreeSurface(boardImg);
	SDL_FreeSurface(arrow);
	TTF_CloseFont(font);
}

int board::checkQuit(SDL_Event event)
{
	if(event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_RALT ||
		event.key.keysym.sym == SDLK_LALT))
		alt--;
	if(event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RALT ||
		event.key.keysym.sym == SDLK_LALT))
		alt++;
	if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
		event.key.keysym.sym == SDLK_F4 && alt > 0))
		return -1;
	return 0;
}

// Text is the string to apply to screen
// x and y are positions, if either is -1 then it means to center on that axis
void board::applyText(string text, int x, int y)
{
	//Set font for message 
	SDL_Surface *message = TTF_RenderText_Blended(font, text.c_str(), fontColor);
	//SDL_Rect structure that defines a rectangle, with the origin at the upper left
	SDL_Rect textPos;
	//text position based on upper left origin and x + y co-odinates
	textPos.x = x; textPos.y = y;
	if(x == -1)
		textPos.x = (screen->w - message->w) / 2;
	if(y == -1)
		textPos.y = (screen->h - message->h) / 2;
	SDL_BlitSurface(message, NULL, screen, &textPos);
	SDL_FreeSurface(message);
}
//Initalise drawable object for main menu
void board::drawMainMenu(int pos)
{
	//Set SDL_BlitSurface
	SDL_BlitSurface(back, NULL, screen, NULL);
	//close font
	TTF_CloseFont(font);
	//set font
	font = TTF_OpenFont("fonts/Iron & Brine.TTF", 108);
	//applytext
	applyText("Tetra", -1, 140);
	//Close font
	TTF_CloseFont(font);
	//set font
	font = TTF_OpenFont("fonts/Iron & Brine.TTF", 48);
	//apply text
	applyText("- Press Enter to Play -", -1, 250);

	int *x = new int;

	if(pos == 0) { TTF_SizeText(font, "- Press Enter to Play -", x, NULL);}

	delete x;
}

int board::mainMenu()
{
	//Call function to initalise drawable objects
	drawMainMenu(0);
	//Default position
	int pos = 0;
	//Create event structure
	SDL_Event event;
	//while update
	while(update() != -1)
	{
		//create variable with contents of pos
		int j = pos;
		//Delay execution to maintain a constant framerate and calculate fps
		SDL_framerateDelay(fps);
		//While currently pending events
		while(SDL_PollEvent(&event))
		{
			if(checkQuit(event) == -1)
				exit(2);
			//if event type is equal to keydown
			if(event.type == SDL_KEYDOWN)
			{
				//if key pressed is UP
				if(event.key.keysym.sym == SDLK_UP)
					//pos = pos - 1
						pos = abs((pos - 1)) % 2;
				//if key pressed is DOWN
				if(event.key.keysym.sym == SDLK_DOWN)//CURRENTLY UNUSED
					pos = (pos + 1) % 2;			 //CURRENTLY UNUSED
				//if key pressed is RETURN or ENTER return pos
				if(event.key.keysym.sym == SDLK_RETURN ||
					event.key.keysym.sym == SDLK_KP_ENTER)
					//return pos
					return pos;
			}
		}
		//if pos is not equal to j (0) then draw main menu
		if(pos != j)
			drawMainMenu(pos);
	}

	return -1;
}


void board::delay(int sec)
{
	int curTime = (unsigned int)time(NULL);
	while(time(NULL) - curTime < sec) { }
	return;
}

void board::drawBoard()
{
	//Set SDL_BlitSurface
	SDL_BlitSurface(back, NULL, screen, NULL);
	//SDL_Rect structure that defines a rectangle, with the origin at the upper left
	SDL_Rect boardRect;
	//set x co ordinate using constant variable
	boardRect.x = xBoard;
	//set y co ordinate using constant variable
	boardRect.y = yBoard;
	//set width and height
	boardRect.w = boardRect.h = 500;
	//Set SDL_BlitSurface
	SDL_BlitSurface(boardImg, NULL, screen, &boardRect);
	//SDL_Rect structure that defines a rectangle, with the origin at the upper left
	SDL_Rect cardLoc;
	//Mix_FreeMusic( music );
	int curCard;
	//For loop that places each card from p2 hand to the screen
	for(int i = 0; i < (int)p2.hand.size(); i++)
	{
		//Sets x axis using constant variable
		cardLoc.x = p1x;
		//Sets y axis using constant variable + cardoff x i
		cardLoc.y = p1y + cardOff * i;
		//curCard stores current card in hand
		curCard = p2.hand[i];
		//Set the colour around the card (boxcolour) to blue, along with setting the same axis at the card
		boxColor(screen, cardLoc.x - 4, cardLoc.y - 4, cardLoc.x + cardX + 3, cardLoc.y + cardY + 3, blue);
		//Adds card to surface
		SDL_BlitSurface(master->list[curCard].img, NULL, screen, &cardLoc);
	}
	//For loop that places each card from p1 to the screen 
	for(int i = 0; i < (int)p1.hand.size(); i++)
	{
		//Sets x axis using constant variable
		cardLoc.x = p2x;
		//Sets y axis using constant variable + cardoff x i
		cardLoc.y = p2y + cardOff * i;
		//curCard stores current card in hand
		curCard = p1.hand[i];
		//Set the colour around the card (boxcolour) to red, along with setting the same axis at the card
		boxColor(screen, cardLoc.x - 4, cardLoc.y - 4, cardLoc.x + cardX + 3, cardLoc.y + cardY + 3, red);
		//Adds back of the card to surface
		SDL_BlitSurface(master->back, NULL, screen, &cardLoc);
	}
	//For loop
	for(int i = 0; i < 9; i++)
	{
		if(active[i].second == -1)
			continue;
		placeCard(i, active[i].first.second, active[i].second);
	}

	if(selected >= 0)
	{
		SDL_Rect arrowPos;
		arrowPos.x = p1x - 32;
		arrowPos.y = p1y + 7 + 65 * selected;
		SDL_BlitSurface(arrow, NULL, screen, &arrowPos);
	}

	char score[2];

	sprintf(score, "%i", p1Score);
	int x, y;
	TTF_SizeText(font, score, &x, &y);
	applyText(score, (93 - x) / 2 + p1x, 30);

	sprintf(score, "%i", p2Score);
	TTF_SizeText(font, score, &x, &y);
	applyText(score, (93 - x) / 2 + p2x, 30);
	update();
}
//Function used for battles
void board::attack(int square, int player)
{
	printf("Performing attack for square %i for player %i\n", square, player);
	//Use two int variables
	int attack1, attack2;
	if((square % 3 == 0 || square % 3 == 1) && active[square + 1].second != -1
		&& active[square + 1].first.second != player)
	{
		//If a battle occurs set attack1 and attack2 to either left, right, up, down
		attack1 = master->list[active[square].second].right;
		attack2 = master->list[active[square + 1].second].left;
		//if attack1 is greated than attack2
		if(attack1 > attack2)
		{
			//card changes owner
			active[square + 1].first.second = player;
			//scores update respectfully
			if(player == 1)
			{
				p1Score++;
				p2Score--;
			}
			else
			{
				p1Score--;
				p2Score++;
			}

		}
		//TRY THIS FOR ALL COMBINATIONS OF LEFT RIGHT UP DOWN
		printf("\tAttacking right.  %i vs %i.  Owner of card is player %i\n", attack1, attack2, active[square + 1].first.second);
	}

	if((square %3 == 1 || square % 3 == 2) && active[square - 1].second != -1
		&& active[square - 1].first.second != player)
	{
		attack1 = master->list[active[square].second].left;
		attack2 = master->list[active[square - 1].second].right;
		if(attack1 > attack2)
		{
			active[square - 1].first.second = player;
			if(player == 1)
			{
				p1Score++;
				p2Score--;
			}
			else
			{
				p1Score--;
				p2Score++;
			}
		}
		printf("\tAttacking left.  %i vs %i.  Owner of card is player %i\n", attack1, attack2, active[square - 1].first.second);
	}

	if((square >= 0 && square <= 5) && active[square + 3].second != -1
		&& active[square + 3].first.second != player)
	{
		attack1 = master->list[active[square].second].bottom;
		attack2 = master->list[active[square + 3].second].top;
		if(attack1 > attack2)
		{
			active[square + 3].first.second = player;
			if(player == 1)
			{
				p1Score++;
				p2Score--;
			}
			else
			{
				p1Score--;
				p2Score++;
			}
		}
		printf("\tAttacking bottom.  %i vs %i.  Owner of card is player %i\n", attack1, attack2, active[square + 3].first.second);
	}

	if((square >= 3 && square <= 8) && active[square - 3].second != -1
		&& active[square - 3].first.second != player)
	{
		attack1 = master->list[active[square].second].top;
		attack2 = master->list[active[square - 3].second].bottom;
		if(attack1 > attack2)
		{
			active[square - 3].first.second = player;
			if(player == 1)
			{
				p1Score++;
				p2Score--;
			}
			else
			{
				p1Score--;
				p2Score++;
			}
		}
		printf("\tAttacking top.  %i vs %i.  Owner of card is player %i\n", attack1, attack2, active[square - 3].first.second);
	}
}
//Function to make a move
int board::makeMove(int player)
{
	//Create even structure
	SDL_Event event;
	if(player == 1)
	{
		//selected = 0
		selected = 0;
		//drawBoard
		drawBoard();

		while(update() != -1)
		{

			SDL_framerateDelay(fps);
			//While not equal to -1 and not polling events
			while(update() != -1 && SDL_PollEvent(&event))
			{
				SDL_framerateDelay(fps);
				if(checkQuit(event) == -1)
					exit(2);
				//if event type is mousebutton down and button down is left mouse button
				else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				{
					//if statements to check which card is selected based on x and y locations
					//updating selected accordingly
					if(event.button.x >= p1x && event.button.x <= p1x + cardX)
					{
						if(event.button.y >= 115 && event.button.y < 184)
							selected = 0;
						else if(event.button.y >= 184 && event.button.y < 253)
							selected = 1;
						else if(event.button.y >= 253 && event.button.y < 322)
							selected = 2;
						else if(event.button.y >= 322 && event.button.y < 391)
							selected = 3;
						else if(event.button.y >= 391 && event.button.y < 511)
							selected = 4;
						else
							return -2;
						//drawboard
						drawBoard();
					}
					else
					{
						//checks square x and y locations of the mouse
						int square = checkArea(event.button.x, event.button.y);
						//if -1 (empty) continue
						if(square == -1)
							continue;

						if(active[square].second != -1)
							continue;
						//sets current player at the owner of card
						active[square].first.first = active[square].first.second = player;
						active[square].second = p2.hand[selected];
						//erases selected item from hand once placed
						p2.hand.erase(p2.hand.begin() + selected);
						//if hand size is not equal to 0
						if(p2.hand.size() != 0)
							//selected = 0
								selected = 0;
						else
							//else selected = -1
							selected = -1;

						//Attack from square with this player
						attack(square, player);
						//drawboard
						drawBoard();
						return 0;
					}
				}
				//Similar to previous but uses keyboard button pressed up and down to chose cards
				else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
				{
					selected = selected - 1;
					if(selected < 0)
						selected = p2.hand.size() - 1;
					drawBoard();
				}
				else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN)
				{
					selected = (selected + 1) % p2.hand.size();
					drawBoard();
				}
			}
		}
	}
	//If the player is computer
	else if(player == 2)
	{

		delay(1);
		//select random card from hand
		selected = rand() % p1.hand.size();
		//select random square on the board
		int square = rand() % 9;
		//while square current owner is not -1
		while(active[square].second != -1)
			//square equals random out of 9
				square = rand()  % 9;
		//Set card owner
		active[square].first.first = active[square].first.second = player;
		active[square].second = p1.hand[selected];
		//erase card from hand
		p1.hand.erase(p1.hand.begin() + selected);
		//set selected to 0
		selected = 0;
		//attack from current square with current player
		attack(square, player);
		//draw board
		drawBoard();
	}
	return 0;
}

int board::checkArea(int x, int y)
{
	// This function will return which square the user clicked inside of starting from 0
	// If outside of squares, return -1
	if(x >= x_1 && x <= x_1 + sWidth)
	{
		if(y >= y_1 && y <= y_1 + sHeight)
			return 0;
		else if(y >= y_2 && y <= y_2 + sHeight)
			return 3;
		else if(y >= y_3 && y <= y_3 + sHeight)
			return 6;
	}
	else if(x >= x_2 && x <= x_2 + sWidth)
	{
		if(y >= y_1 && y <= y_1 + sHeight)
			return  1;
		else if(y >= y_2 && y <= y_2 + sHeight)
			return 4;
		else if(y >= y_3 && y <= y_3 + sHeight)
			return 7;
	}
	else if(x >= x_3 && x <= x_3 + sWidth)
	{
		if(y >= y_1 && y <= y_1 + sHeight)
			return 2;
		else if(y >= y_2 && y <= y_2 + sHeight)
			return 5;
		else if(y >= y_3 && y <= y_3 + sHeight)
			return 8;
	}
	return -1;
}

pair<int,int> board::findXY(int square)
{
	//A table lookup for all of the card x, y positions
	//All calculations are based on global variables and returned in a pair
	int xDim, yDim;
	switch(square)
	{
	case 0:
		xDim = x_1 + xOff;
		yDim = y_1 + yOff;
		break;

	case 1:
		xDim = x_2 + xOff;
		yDim = y_1 + yOff;
		break;

	case 2:
		xDim = x_3 + xOff;
		yDim = y_1 + yOff;
		break;

	case 3:
		xDim = x_1 + xOff;
		yDim = y_2 + yOff;
		break;

	case 4:
		xDim = x_2 + xOff;
		yDim = y_2 + yOff;
		break;

	case 5:
		xDim = x_3 + xOff;
		yDim = y_2 + yOff;
		break;

	case 6:
		xDim = x_1 + xOff;
		yDim = y_3 + yOff;
		break;

	case 7:
		xDim = x_2 + xOff;
		yDim = y_3 + yOff;
		break;

	case 8:
		xDim = x_3 + xOff;
		yDim = y_3 + yOff;
		break;
	}

	return pair<int,int>(xDim, yDim);
}

int board::placeCard(int square, int player, int selectedCard)
{
	// Returns the x and y coordinates for the top left corner of the card -- used in rect
	pair<int,int> dims = findXY(square);  

	// If statements to change background color based on player
	if(player == 1)
		boxColor(screen, dims.first - 4, dims.second - 4, dims.first + 96, dims.second + 123, 0x0000FFFF);
	else if(player == 2)
		boxColor(screen, dims.first - 4, dims.second - 4, dims.first + 96, dims.second + 123, 0xFF0000FF);

	// Rect for blitting card from hand
	SDL_Rect card;
	card.x = dims.first;
	card.y = dims.second;
	SDL_BlitSurface(master->list[selectedCard].img, NULL, screen, &card);

	return 0;
}

void board::dispWinner()
{
	char winner[100];
	//If player1 score is higher than player2 print winner, viceversa, and if not it's a draw
	if(p1Score > p2Score)
		sprintf(winner, "%s wins!", p2.name.c_str());
	else if(p2Score > p1Score)
		sprintf(winner, "%s wins!", p1.name.c_str());
	else
		sprintf(winner, "It's a draw");
	//Apply winner text
	applyText(winner, -1, 30);
	SDL_Event event;
	while(update() != -1)
		while(update() != -1 && SDL_PollEvent(&event))
			//Quit application by pressing a button or mouse click
				if(event.type == SDL_QUIT || (event.type == SDL_MOUSEBUTTONDOWN || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)))
					return;
}

card::card(string name, int top, int right, int bottom, int left, int level, string imgFile)
{
	//Initializes card for card list.  Needs attack values, name, and level
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->left = left;
	this->level = level;
	this->name = name;
	this->file = imgFile;
	this->img = loadImage(imgFile);
}

card::~card() { }


cardList::cardList()
{
	// Creates the list of all cards. 
	// This function will load the list of cards from a file.  File format should be as follows:
	// 1: (# of cards in deck)
	// 2-#: Name   top   right   bottom   left   level   imgPath

	// Later the file can also specify a relative path to the card image (including file name or with pictures numbered).
	flag = 0;
	string buffer, name;
	char *nameBuffer, *imgFile;
	int top, right, bottom, left, level;
	//Used to read in list of cards from file cards.txt
	ifstream infile(filename.c_str()); 
	//process infile
	while(getline(infile, buffer))
	{
		nameBuffer = (char*)malloc(sizeof(char) * buffer.size());
		imgFile = (char*)malloc(sizeof(char) * buffer.size());
		printf("Preparing to process %s\n", buffer.c_str());
		top = right = bottom = left = level = -1;
		if(buffer == "")
		{
			flag = 1;
			return;
		}

		if(sscanf(buffer.c_str(), "%s %i %i %i %i %i %s", nameBuffer, &top, &right, &bottom, &left, &level, imgFile) != 7)
		{
			flag = 1;
			return;
		}
		name = nameBuffer;
		free(nameBuffer);
		int pos = name.find("_", 0);
		while(pos != -1)
		{
			name.replace(pos, 1, " ");
			pos = name.find("_", pos);
		}
		card newCard(name, top, right, bottom, left, level, imgFile);
		list.push_back(newCard);
		if(list[list.size() - 1].img == NULL)
		{
			flag = 2;
			printf("Error loading %s\n", imgFile);
			return;
		}
		else
			printf("Loaded %s\n", imgFile);

		free(imgFile);
	}

	infile.close();
	back = loadImage("img/cBack.png");
}

cardList::~cardList()
{
	//create cardlist
	SDL_FreeSurface(back);
	for(int i = 0; i < (int)list.size(); i++)
		SDL_FreeSurface(list[i].img);
}
//error message, and prints it to stderr
int cardList::perror()
{
	switch(flag)
	{
	case -1:
		printf("Unable to allocate memory!\n");
		break;

	case 0:
		printf("No errors reported\n");
		break;

	case 1:
		printf("Error in formatting of %s:\nNumber of cards not specified on first line of file\n", filename.c_str());
		break;

	case 2:
		printf("Error loading image files for cards.  Please check paths listed in %s\n", filename.c_str());
		break;
	}
	return flag;
}
