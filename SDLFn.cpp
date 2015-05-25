#include <SDL/SDL_image.h>
#include "SDL/SDL_mixer.h"
#include <SDL/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include "Tetra.h"

using namespace std;

int initSDL()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) //Initialize main SDL library
	{
		printf("Failed to initialize SDL\n");
		return 0;
	}

	/*    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) //Initialize sound extension
	{
	printf("Failed to initialize mixer library\n");
	return 0;
	}

	Mix_Init(MIX_INIT_MP3); //Init SDL_mixer to decode mp3's
	*/
	if(TTF_Init() == -1) //Initialize fonts extension
	{
		printf("Error initializg TTF library: %s\n", TTF_GetError());
		return 0;
	}
	return 0;
}

void cleanupSDL()
{
	TTF_Quit();
	//    Mix_Quit();
	//    Mix_CloseAudio();
	SDL_Quit();
}

SDL_Surface* loadImage(string filename)
{
	SDL_Surface* loadedImage = NULL;
	//    SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load(filename.c_str());
	return loadedImage;


}
