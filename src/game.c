#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "perlin_noise.h"

double get_terrain_height(int x) {
	return get_height(x, 256) * 0.6 +
		   get_height(x, 128) * 0.25 +
		   get_height(x, 64)  * 0.125 +
		   get_height(x, 32)  * 0.0625;
}

void Game_start(SDL_Renderer *renderer) {
	int delta = 0;

	bool is_running = true;
	SDL_Event event;
	while(is_running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					is_running = false;
					break;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0,0,0,0xff);
		SDL_RenderClear(renderer);
		for (int x = 0; x < 1000; x++)
		{
			filledCircleRGBA(renderer, x, 600 - get_terrain_height(x+delta)*300, 1, 0xff, 0xff, 0xff, 0xff);
		}
		delta++;

		SDL_RenderPresent(renderer);
	}
}
