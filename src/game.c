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
#include "menu.h"
#include "button.h"
#include "particle.h"

TTF_Font *font_large;
TTF_Font *font_small;

static Button buttons[] = {
	{.text = "Retry"},
	{.text = "New Game"},
	{.text = "Exit"},
};
static int button_count = 3;

GameState init_game(SDL_Renderer *renderer, int *terrain_seed) {
	Lander lander = init_lander(renderer);
	Camera camera = {
		.position = {0, 0},
		.zoom = 1,
		.renderer = renderer,
		.width = 0,
		.height = 0,
	};

	GameState state = {
		.lander = lander, 
		.camera = camera,
		.delta_time = 0,
		.game_over = false,
		.successfull = false
	};

	font_large = TTF_OpenFont("assets/PressStart2P.ttf", 40);
	if(font_large == NULL) {
		SDL_Log("Error while opening font: %s", TTF_GetError());
		exit(1);
	}
	font_small = TTF_OpenFont("assets/PressStart2P.ttf", 24);
	if(font_small == NULL) {
		SDL_Log("Error while opening font: %s", TTF_GetError());
		exit(1);
	}

	init_terrain(terrain_seed);

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
	if(!state->game_over) {
		update_lander(&state->lander, state->delta_time);
	}
	update_particles(&state->lander.particle_system, state->delta_time);
	update_camera(&state->camera, state->lander.position, state->delta_time);

	//Rendering
	SDL_SetRenderDrawColor(state->camera.renderer, 0,0,0,0xff);
	SDL_RenderClear(state->camera.renderer);

	if(!state->game_over || state->successfull) {
		render_lander(&state->camera, &state->lander);
	}
	render_particles(&state->camera, &state->lander.particle_system);
	render_terrain(&state->camera);
	display_dashboard(&state->camera, &state->lander);

	if(state->game_over && state->game_over_dealy <= 0) {
		render_game_over(&state->camera);
	}
	else if(state->game_over) {
		state->game_over_dealy -= state->delta_time;
	}
	
	SDL_RenderPresent(state->camera.renderer);

	state->delta_time = (SDL_GetPerformanceCounter() - time)/SDL_GetPerformanceFrequency();
}

void render_game_over(Camera *camera) {
	SDL_Renderer *renderer = camera->renderer;
	SDL_Rect container = {
		(camera->width - 500) / 2,
		(camera->height - 400) / 2,
		500,
		400
	};

	SDL_SetRenderDrawColor(renderer, 0x39, 0x4a, 0x50, 0xff);
	SDL_RenderFillRect(renderer, &container);

	SDL_Color text_color = {255, 255, 255, 255};

	SDL_Surface *text_s = TTF_RenderUTF8_Solid(font_large, "Game Over", text_color);
	SDL_Texture *text_t = SDL_CreateTextureFromSurface(renderer, text_s);

	SDL_Rect dst = {
		container.x + (container.w / 2) - (text_s->w / 2),
		container.y + 40,
		text_s->w,
		text_s->h
	};

	SDL_RenderCopy(renderer, text_t, NULL, &dst);

	for (int i = 0; i < button_count; i++)
	{
		buttons[i].rect.w = text_s->w;
		buttons[i].rect.h = 70;

		buttons[i].rect.x = container.x + (container.w / 2) - (buttons[i].rect.w / 2);
		buttons[i].rect.y = container.y + text_s->h + (container.h / 2) - (((buttons[i].rect.h * button_count) + (5 * (button_count - 1))) / 2) + ((buttons[i].rect.h + 5) * i);
		render_button(renderer, font_small, buttons + i);
	}
	
	SDL_FreeSurface(text_s);
	SDL_DestroyTexture(text_t);
}

Screen game_events(SDL_Event event, GameState *state) {
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
		case SDL_MOUSEMOTION: {
			SDL_Point point = {
				event.motion.x,
				event.motion.y
			};

			for (int i = 0; i < button_count; i++)
			{
				buttons[i].hover = SDL_PointInRect(&point, &buttons[i].rect);
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN: 
			if(state->game_over && event.button.button == SDL_BUTTON_LEFT) {
				SDL_Point point = {
					event.button.x,
					event.button.y,
				};

				if(SDL_PointInRect(&point, &buttons[0].rect)) {
					SDL_Renderer *renderer = state->camera.renderer;
					destroy_game(state);
					*state = init_game(renderer, &TERRAIN_SEED);
				}
				else if(SDL_PointInRect(&point, &buttons[1].rect)) {
					SDL_Renderer *renderer = state->camera.renderer;
					destroy_game(state);
					*state = init_game(renderer, NULL);
				}
				else if(SDL_PointInRect(&point, &buttons[2].rect)){
					return MENU;
				}
			}
			break;
		case SDL_USEREVENT:
			switch (event.user.code)
			{
				case DEATH_EVENT_CODE:
					state->game_over = true;
					state->game_over_dealy = 2;
					SDL_Rect explosion_rect = {0, 0, 64, 64};
					Vector2 explosion_velocity = {70, 0};
					SDL_Color start_color = {0xda, 0x86, 0x3e, 0xff};
					SDL_Color end_color = {0xe8, 0xc1, 0x70, 0x00};
					bulk_add_particles(&state->lander, 1000, 10, explosion_rect, 2, explosion_velocity, 360, start_color, end_color);
					break;
				case SUCCESS_EVENT_CODE:
					state->game_over = true;
					state->successfull = true;
					state->game_over_dealy = 2;
					break;
			}
			break;
	}
	return GAME;
}

void destroy_game(GameState *state) {
	if(!state->destroyed) {
		destroy_lander(&state->lander);
		TTF_CloseFont(font_large);
		TTF_CloseFont(font_small);
		state->destroyed = true;
	}
}
