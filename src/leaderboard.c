#include <SDL.h>
#include <SDL_ttf.h>

#include "leaderboard.h"
#include "file_handler.h"
#include "text_io.h"
#include "button.h"
#include "menu.h"

#include "debugmalloc.h"

static Score *score = NULL;
static int count = 0;
static int start = 0;
static TTF_Font *font;
static TTF_Font *button_font;

static Button buttons[] = {
	{
		.text = "<",
		.rect = {
			.w = 80,
			.h = 80
		}
	},
	{
		.text = "Back",
		.rect = {
			.w = 200,
			.h = 80
		}
	},
	{
		.text = ">",
		.rect = {
			.w = 80,
			.h = 80
		}
	}
};
static int button_count = 3;
static int offsets[] = {
	0,
	90,
	300
};


void init_leaderboard() {
	font = TTF_OpenFont("assets/PressStart2P.ttf", 16);
	if(font == NULL) {
		SDL_Log("Error while opening font: %s", TTF_GetError());
		exit(1);
	}

	button_font = TTF_OpenFont("assets/PressStart2P.ttf", 24);
	if(button_font == NULL) {
		SDL_Log("Error while opening font: %s", TTF_GetError());
		exit(1);
	}

	count = read_scores(&score);
}

void render_leaderboard(SDL_Renderer *renderer) {
	int offset = 40;
	SDL_Color text_color = {255, 255, 255, 255};
	SDL_Rect dst = {
		.x = 0, 
		.y = 0
	};

	SDL_GetRendererOutputSize(renderer, &dst.w, &dst.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	render_text_centered(renderer, &dst, "Index Score Name", font, text_color, 60);
	for (int i = start; i < start + 10 && i < count; i++)
	{
		char line[100];
		snprintf(line, 100, "%2d. %6d  %s", i+1, score[i].score, score[i].name);
		render_text_centered(renderer, &dst, line, font, text_color, 100 + offset * (i - start));
	}

	for(int i = 0; i < button_count; i++) {
		buttons[i].rect.y = 100 + 10 * offset;

		buttons[i].rect.x = (dst.w - 380) / 2 + offsets[i];
		render_button(renderer, button_font, buttons + i);
	}

	SDL_RenderPresent(renderer);
}

Screen leaderboard_events(SDL_Event event) {
	switch(event.type) {
		case SDL_MOUSEMOTION: {
			SDL_Point point = {
				event.motion.x,
				event.motion.y
			};

			for (int i = 0; i < button_count; i++)
			{
				buttons[i].hover = SDL_PointInRect(&point, &buttons[i].rect);
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN: 
			if(event.button.button == SDL_BUTTON_LEFT) {
				SDL_Point point = {
					event.button.x,
					event.button.y,
				};

				if(SDL_PointInRect(&point, &buttons[0].rect)) {
					if(start >= 10) start -= 10;
				}
				if(SDL_PointInRect(&point, &buttons[1].rect)) {
					return MENU;
				}
				if(SDL_PointInRect(&point, &buttons[2].rect)) {
					if(start + 10 < count) start += 10;
				}
			}
			break;
		default:
			break;
	}
	return LEADERBOARD;
}

void destroy_leaderboard() {
	if(score != NULL) {
		free(score);
		score = NULL;
		TTF_CloseFont(font);
		TTF_CloseFont(button_font);
	}
}