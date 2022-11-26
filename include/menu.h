#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef enum Screen {
	MENU,
	GAME,
	LEADERBOARD
} Screen;

void init_menu();
void destroy_menu();
void render_menu(SDL_Renderer *renderer);
Screen menu_events(SDL_Event event);
#endif