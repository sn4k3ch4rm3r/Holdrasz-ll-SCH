#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "game.h"
#include "menu.h"
#include "leaderboard.h"

#include "debugmalloc.h"

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
	
	TTF_Init();
	
	SDL_RenderClear(renderer);


	//Set up the state
	Screen current_screen = MENU;
	GameState game_state;

	init_menu();

	bool is_running = true;
	SDL_Event event;

	//Main loop
	while (is_running) {
	
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					is_running = false;
					break;
				
				default:
					switch (current_screen) {
						case GAME:
							current_screen = game_events(event, &game_state);
							if(current_screen == MENU) {
								destroy_game(&game_state);
							}
							break;
						case MENU:
							current_screen = menu_events(event);
							if(current_screen == GAME) {
								game_state = init_game(renderer, NULL);
							}
							else if(current_screen == LEADERBOARD) {
								init_leaderboard();
							}
							break;
						case LEADERBOARD:
							current_screen = leaderboard_events(event);
							if(current_screen == MENU) {
								destroy_leaderboard();
							}
							break;
					}
					break;
			}
		}

		switch(current_screen) {
			case MENU:
				render_menu(renderer);
				break;
			case GAME:
				update_game(&game_state);
				break;
			case LEADERBOARD:
				render_leaderboard(renderer);
				break;
		}
	}

	destroy_menu();
	destroy_game(&game_state);
	destroy_leaderboard();
	
	SDL_Quit();

	return 0;
}

