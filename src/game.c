#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include "game.h"
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

GameState init_game(SDL_Renderer *renderer) {
	Lander lander = init_lander(renderer);
	Camera camera = {
		{0, 0},
		1,
		renderer,
		0,
		0
	};

	GameState state = {
		lander, 
		camera
	};

	return state;
}

void game_loop(GameState *state) {
	double dt = 0;
	bool is_running = true;

	while(is_running) {
		double time = SDL_GetPerformanceCounter();
		
		//Keep camera and window size in snyc
		int screen_width, screen_height;
		SDL_GetRendererOutputSize(state->camera.renderer, &screen_width, &screen_height);

		state->camera.width = screen_width;
		state->camera.height = screen_height;

		//Event
		is_running = game_events(&state->lander);

		//Updates
		update_lander(&state->lander, dt);
		update_camera(&state->camera, state->lander.position, dt);

		//Rendering
		SDL_SetRenderDrawColor(state->camera.renderer, 0,0,0,0xff);
		SDL_RenderClear(state->camera.renderer);
		for (int x = 0; x < state->camera.width; x++)
		{
			Vector2 point = {
				x, 
				get_terrain_height((x + state->camera.position.x)/state->camera.zoom)*50
			};
			Vector2 render = get_screen_coordinates(&state->camera, point);
			pixelRGBA(state->camera.renderer, x, render.y, 0xff, 0xff, 0xff, 0xff);
		}

		render_lander(&state->camera, &state->lander);
		pixelRGBA(state->camera.renderer, state->camera.width/2, state->camera.height/2, 0xff, 0,0,0xff);
		SDL_RenderPresent(state->camera.renderer);

		dt = (SDL_GetPerformanceCounter() - time)/SDL_GetPerformanceFrequency();
	}
	destroy_lander(&state->lander);
}

bool game_events(Lander *lander) {
	bool is_running = true;
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				is_running = false;
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				bool engine_state = event.key.state == SDL_PRESSED;
				switch(event.key.keysym.sym) {
					case SDLK_w:
						lander->engines[MAIN_ENGINE] = engine_state;
						break;
					case SDLK_a:
						lander->engines[RIGHT_ENGINE] = engine_state;
						break;
					case SDLK_d:
						lander->engines[LEFT_ENGINE] = engine_state;
						break;
					case SDLK_k:
						lander->engines[ROTATE_CW] = engine_state;
						break;
					case SDLK_j:
						lander->engines[ROTATE_CCW] = engine_state;
						break;
				}
				break;
			}
		}
	}
	return is_running;
}