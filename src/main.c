#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "main.h"
#include "game.h"
#include "lander.h"

int main(int argc, char* argv[]) {
	//Set up SDL
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

	//Set up the state
	Screen current_screen = GAME;
	GameState game_state = init_game(renderer);

	bool is_running = true;
	SDL_Event event;

	//Main loop
	while (is_running) {
		while(SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				is_running = false;
				destroy_lander(&game_state.lander);
				break;
			
			default:
				game_events(event, &game_state);
				break;
			}
		}

		switch(current_screen) {
			case GAME:
				update_game(&game_state);
				break;
		}
	}

	SDL_Quit();

	return 0;
}

