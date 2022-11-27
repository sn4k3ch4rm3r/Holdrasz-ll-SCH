#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "text_io.h"

bool valid(char *text) {
    int len = strlen(text);
    for (int i = 0; i < len; i++)
    {
        if(!isalnum(text[i])) {
            return false;
        }
    }
    return true;
}

/// @brief From INFOC
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 10;
    int maxh = teglalap.h - 2;
 
    dest[0] = '\0';
 
    bool enter = false;
    bool kilep = false;
 
    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x + 10, teglalap.y + teglalap.h / 2 - felirat->h / 2, felirat->w+20 < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 10, teglalap.y + 5, teglalap.y + teglalap.h - 6, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);
 
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;
 
            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz && valid(event.text.text)) {
                    strcat(dest, event.text.text);
                }
 
                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;
 
            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;
 
            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }
 
    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

SDL_Rect render_text_centered(SDL_Renderer *renderer, SDL_Rect *container, char *text, TTF_Font *font, SDL_Color text_color, double y_offset) {
	SDL_Surface *text_s = TTF_RenderUTF8_Solid(font, text, text_color);
	SDL_Texture *text_t = SDL_CreateTextureFromSurface(renderer, text_s);

	SDL_Rect dst = {
		container->x + (container->w / 2) - (text_s->w / 2),
		container->y + y_offset,
		text_s->w,
		text_s->h
	};

	SDL_RenderCopy(renderer, text_t, NULL, &dst);
	SDL_FreeSurface(text_s);
	SDL_DestroyTexture(text_t);

	return dst;
}