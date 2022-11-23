#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include "game.h"
#include "terrain.h"
#include "vector.h"
#include "lander.h"
#include "camera.h"
#include "events.h"

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
		.lander = lander, 
		.camera = camera,
		.delta_time = 0,
		.game_over = false
	};

	return state;
}

void update_game(GameState *state) {
	double time = SDL_GetPerformanceCounter();
	
	//Keep camera and window size in snyc
	int screen_width, screen_height;
	SDL_GetRendererOutputSize(state->camera.renderer, &screen_width, &screen_height);

	state->camera.width = screen_width;
	state->camera.height = screen_height;

	//Updates
	update_lander(&state->lander, state->delta_time);
	update_camera(&state->camera, state->lander.position, state->delta_time);

	//Rendering
	SDL_SetRenderDrawColor(state->camera.renderer, 0,0,0,0xff);
	SDL_RenderClear(state->camera.renderer);

	render_lander(&state->camera, &state->lander);
	render_terrain(&state->camera);
	display_dashboard(&state->camera, &state->lander);

	SDL_RenderPresent(state->camera.renderer);

	state->delta_time = (SDL_GetPerformanceCounter() - time)/SDL_GetPerformanceFrequency();
}

void game_events(SDL_Event event, GameState *state) {
	switch(event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP: {
			bool engine_state = event.key.state == SDL_PRESSED;
			switch(event.key.keysym.sym) {
				case SDLK_w:
					state->lander.engines[MAIN_ENGINE] = engine_state;
					break;
				case SDLK_a:
					state->lander.engines[RIGHT_ENGINE] = engine_state;
					break;
				case SDLK_d:
					state->lander.engines[LEFT_ENGINE] = engine_state;
					break;
				case SDLK_k:
					state->lander.engines[ROTATE_CW] = engine_state;
					break;
				case SDLK_j:
					state->lander.engines[ROTATE_CCW] = engine_state;
					break;
			}
			break;
		}
		case SDL_USEREVENT:
			switch (event.user.code)
			{
				case DEATH_EVENT_CODE:
					state->game_over = true;
			}
			break;
	}
}

void destroy_game(GameState *state) {
	destroy_lander(&state->lander);
}
