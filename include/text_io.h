#ifndef TEXTIO_H
#define TEXTIO_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);
SDL_Rect render_text_centered(SDL_Renderer *renderer, SDL_Rect *container, char *text, TTF_Font *font, SDL_Color text_color, double y_offset);
#endif