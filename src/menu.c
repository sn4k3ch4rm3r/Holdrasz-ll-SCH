#include <SDL.h>
#include <SDL_ttf.h>

#include "menu.h"
#include "button.h"

Button buttons[] = {
	{
		.text = "New Game", 
	},
	{
		.text = "Leaderboard"
	}
};
const int button_count = 2;
const margin = 20;
const SDL_Color bg_color = {129, 151, 150, 255};
const SDL_Color hover_color = {168, 202, 88, 255};
const SDL_Color fg_color = {255, 255, 255, 255};

TTF_Font *font;

void init_menu() {
	font = TTF_OpenFont("assets/PressStart2P.ttf", 24);
	if(font == NULL) {
		SDL_Log("Error while opening font: %s", TTF_GetError());
		exit(1);
	}

	SDL_Rect rect = {
		.w = 350,
		.h = 80
	};
	for (int i = 0; i < button_count; i++)
	{
		buttons[i].rect = rect;
		buttons[i].bg = bg_color;
		buttons[i].fg = fg_color;
	}
	
}

void destroy_menu() {
	TTF_CloseFont(font);
}

void render_menu(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderClear(renderer);

	int screen_w, screen_h;
	SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);

	for (int i = 0; i < button_count; i++)
	{
		buttons[i].rect.x = (screen_w / 2) - (buttons[i].rect.w / 2);
		buttons[i].rect.y = (screen_h / 2) - (((buttons[i].rect.h * button_count) + (margin * (button_count - 1))) / 2) + ((buttons[i].rect.h + margin) * i);
		render_button(renderer, font, buttons + i);
	}

	SDL_RenderPresent(renderer);
}

Screen menu_events(SDL_Event event) {
	switch(event.type) {
		case SDL_MOUSEMOTION: {
			SDL_Point point = {
				event.motion.x,
				event.motion.y
			};

			for (int i = 0; i < button_count; i++)
			{
				if(SDL_PointInRect(&point, &buttons[i].rect)){
					buttons[i].bg = hover_color;
				}
				else {
					buttons[i].bg = bg_color;
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN: 
			if(event.button.button == SDL_BUTTON_LEFT) {
				SDL_Point point = {
					event.button.x,
					event.button.y,
				};

				for (int i = 0; i < button_count; i++)
				{
					if(SDL_PointInRect(&point, &buttons[i].rect)) {
						return i+1;
					}
				}
			}
			break;
		default:
			break;
	}
	return MENU;
}