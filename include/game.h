#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "lander.h"
#include "camera.h"
#include "menu.h"

typedef struct GameState {
	Lander lander;
	Camera camera;
	double delta_time;
	double game_over_dealy;
	bool game_over;
	bool successfull;
	bool destroyed;
} GameState;

/// @brief Sets up the default parameters for the lander, the camera and the world.
/// @param renderer Used for rendering the game
/// @return GameState containing the lander and camera structs
GameState init_game(SDL_Renderer *renderer, int *terrain_seed);

void update_game(GameState *state);
Screen game_events(SDL_Event event, GameState *state);
void destroy_game(GameState *state);
void render_game_over(Camera *camera);
#endif
