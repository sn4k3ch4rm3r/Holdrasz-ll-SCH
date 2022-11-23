#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct Button {
	SDL_Rect rect;
	SDL_Color bg;
	SDL_Color fg;
	char *text;
} Button;

void render_button(SDL_Renderer *renderer, TTF_Font *font, Button *button);

#endif