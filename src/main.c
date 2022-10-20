#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "game.h"

int main(int argc, char* argv[]) {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow("HoldraszálláSCH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, 0);
	if(window == NULL) {
		SDL_Log("Error while creating window: %s", SDL_GetError());
		exit(1);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if(renderer == NULL) {
		SDL_Log("Error while creating renderer: %s", SDL_GetError());
	}
	SDL_RenderClear(renderer);

	Game_start(renderer);

	SDL_Quit();

	return 0;
}