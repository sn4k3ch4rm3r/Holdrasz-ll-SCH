#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "perlin_noise.h"

double get_terrain_height(int x) {
	return noise(x, 256) * 0.6 +
		   noise(x, 128) * 0.25 +
		   noise(x, 64)  * 0.125 +
		   noise(x, 32)  * 0.0625;
}

void game_loop(SDL_Renderer *renderer) {
	double delta = 0;
	double dt = 0;

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
		
		delta += 250 * dt;

		double time = clock();
		SDL_SetRenderDrawColor(renderer, 0,0,0,0xff);
		SDL_RenderClear(renderer);
		for (int x = 1; x < 1000; x++)
		{
			pixelRGBA(renderer, x, 600 - get_terrain_height(x+delta)*300, 0xff, 0xff, 0xff, 0xff);
		}
		dt = (clock() - time) / CLOCKS_PER_SEC;

		SDL_RenderPresent(renderer);
	}
}
