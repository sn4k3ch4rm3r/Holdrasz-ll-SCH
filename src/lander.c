#include <SDL.h>
#include <SDL_image.h>

#include "lander.h"
#include "vector.h"
#include "camera.h"

#include "LunarModule.xpm"

void render_lander(Camera *camera, Lander *lander) {
	const int width = 64;
	const int height = 64;
	SDL_Texture *texture = SDL_CreateTextureFromSurface(camera->renderer, IMG_ReadXPMFromArray(lunar_module));

	Vector2 screen_coords = get_screen_coordinates(camera, lander->position);
	SDL_Rect dst = {screen_coords.x, screen_coords.y, width * camera->zoom, height * camera->zoom};

	SDL_RenderCopyEx(camera->renderer, texture, NULL, &dst, lander->rotation, NULL, SDL_FLIP_NONE);
	SDL_DestroyTexture(texture);
}