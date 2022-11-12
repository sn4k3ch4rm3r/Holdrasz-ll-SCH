#ifndef GAME_H
#define GAME_H

#include "lander.h"
#include "camera.h"

typedef struct GameState {
	Lander lander;
	Camera camera;
} GameState;

/// @brief Sets up the default parameters for the lander, the camera and the world.
/// @param renderer Used for rendering the game
/// @return GameState containing the lander and camera structs
GameState init_game(SDL_Renderer *renderer);

/// @brief Handles events, updates and rendering of the game.
/// @param state GameState returned by init_game()
void game_loop(GameState *state);

/// @brief Handles the game events.
/// @param state Current game state.
/// @return true if the game is still running, false if the user closed the window.
bool game_events(Lander *lander);

#endif
