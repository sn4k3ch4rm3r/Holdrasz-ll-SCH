#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);

#endif