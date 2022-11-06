#ifndef GAME_H
#define GAME_H

#include "lander.h"
#include "camera.h"

typedef struct GameState {
	Lander lander;
	Camera camera;
} GameState;

GameState init_game(SDL_Renderer *renderer);
void game_loop(GameState *state);

/// @brief Handles the game events.
/// @param state Current game state.
/// @return true if the game is still running, false if the user closed the window.
bool game_events(Lander *lander);

#endif
