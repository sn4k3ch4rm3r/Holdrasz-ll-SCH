#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "button.h"

void render_button(SDL_Renderer *renderer, TTF_Font *font, Button *button) {

	SDL_SetRenderDrawColor(renderer, button->bg.r, button->bg.g, button->bg.b, button->bg.a);
	SDL_RenderFillRect(renderer, &button->rect);

	SDL_Surface *text_s = TTF_RenderUTF8_Solid(font, button->text, button->fg);
	SDL_Texture *text_t = SDL_CreateTextureFromSurface(renderer, text_s);

	SDL_Rect text_rect = {
		button->rect.x + (button->rect.w / 2) - (text_s->w / 2),
		button->rect.y + (button->rect.h / 2) - (text_s->h / 2),
		text_s->w,
		text_s->h
	};

	SDL_RenderCopy(renderer, text_t, NULL, &text_rect);
    SDL_FreeSurface(text_s);
    SDL_DestroyTexture(text_t);
}