#include <SDL.h>
#include <SDL_ttf.h>

#include "leaderboard.h"
#include "file_handler.h"
#include "text_io.h"

#include "debugmalloc.h"

Score *score = NULL;
int count = 0;
TTF_Font *font;

void init_leaderboard() {
	font = TTF_OpenFont("assets/PressStart2P.ttf", 16);
	if(font == NULL) {
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
	
	int start = 0;
	render_text_centered(renderer, &dst, "Index Score Name", font, text_color, 60);
	for (int i = start; i < start + 10 && i < count; i++)
	{
		char line[100];
		snprintf(line, 100, "%2d. %6d  %s", i+1, score[i].score, score[i].name);
		render_text_centered(renderer, &dst, line, font, text_color, 100 + offset * (i - start));
	}

	SDL_RenderPresent(renderer);
}

void destroy_leaderboard() {
	if(score != NULL) {
		free(score);
		score = NULL;
	}
	TTF_CloseFont(font);
}