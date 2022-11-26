#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "button.h"

const SDL_Color bg_color = {129, 151, 150, 255};
const SDL_Color hover_color = {168, 202, 88, 255};
const SDL_Color fg_color = {255, 255, 255, 255};

void render_button(SDL_Renderer *renderer, TTF_Font *font, Button *button) {
	SDL_Color bg = button->hover ? hover_color : bg_color;

	SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
	SDL_RenderFillRect(renderer, &button->rect);

	SDL_Surface *text_s = TTF_RenderUTF8_Solid(font, button->text, fg_color);
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