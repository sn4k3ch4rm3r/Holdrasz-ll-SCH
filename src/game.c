#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include "perlin_noise.h"
#include "vector.h"
#include "lander.h"
#include "camera.h"

double get_terrain_height(int x) {
	return noise(x, 256) * 0.6 +
		   noise(x, 128) * 0.25 +
		   noise(x, 64)  * 0.125 +
		   noise(x, 32)  * 0.0625;
}

void game_loop(SDL_Renderer *renderer) {
	double dt = 0;

	Lander lander = {
		{10, 110},
		{100, 0},
		-90,
		0,
		100,
		1000
	};

	Camera camera = {
		{0, 0},
		1,
		renderer,
		0,
		0
	};

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

		double time = SDL_GetPerformanceCounter();
		int screen_width, screen_height;
		SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);

		camera.width = screen_width;
		camera.height = screen_height;

		SDL_SetRenderDrawColor(renderer, 0,0,0,0xff);
		SDL_RenderClear(renderer);
		for (int x = 0; x < camera.width; x++)
		{
			pixelRGBA(renderer, x, get_camera_height(&camera) - get_terrain_height((x + camera.position.x)/camera.zoom)*300*camera.zoom, 0xff, 0xff, 0xff, 0xff);
		}

		render_lander(&camera, &lander);
		SDL_RenderPresent(renderer);

		dt = (SDL_GetPerformanceCounter() - time)/SDL_GetPerformanceFrequency();
	}
}
