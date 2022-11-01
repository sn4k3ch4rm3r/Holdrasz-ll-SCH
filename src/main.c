#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "game.h"

int main(int argc, char* argv[]) {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow("HoldraszálláSCH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 800, 0);
	if(window == NULL) {
		SDL_Log("Error while creating window: %s", SDL_GetError());
		exit(1);
	}
	SDL_SetWindowResizable(window, SDL_TRUE);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
		SDL_Log("Error while creating renderer: %s", SDL_GetError());
	}
	SDL_RenderClear(renderer);

	game_loop(renderer);

	SDL_Quit();

	return 0;
}