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

	Lander lander = init_lander();
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
		//Keep camera and window size in snyc
		double time = SDL_GetPerformanceCounter();
		int screen_width, screen_height;
		SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);

		camera.width = screen_width;
		camera.height = screen_height;

		//Event handling
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					is_running = false;
					break;

				case SDL_KEYDOWN:
				case SDL_KEYUP: {
					bool on = event.key.state == SDL_PRESSED;
					switch(event.key.keysym.sym) {
						case SDLK_w:
							set_engine(&lander, MAIN_ENGINE, on);
							break;
						case SDLK_a:
							set_engine(&lander, RIGHT_ENGINE, on);
							break;
						case SDLK_d:
							set_engine(&lander, LEFT_ENGINE, on);
							break;
						case SDLK_k:
							set_engine(&lander, ROTATE_CW, on);
							break;
						case SDLK_j:
							set_engine(&lander, ROTATE_CCW, on);
							break;
					}
					break;
				}
			}
		}

		//Update physics
		update_lander(&lander, dt);

		const int screen_buffer = 100;
		Vector2 lander_render_pos = get_screen_coordinates(&camera, lander.position);
		if(lander_render_pos.x > screen_width - screen_buffer - 64){
			camera.position.x += (lander_render_pos.x - screen_width + screen_buffer + 64) / 7;
		}

		//Rendering
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
