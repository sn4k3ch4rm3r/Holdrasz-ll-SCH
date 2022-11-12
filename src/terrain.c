#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>

#include "camera.h"
#include "vector.h"

const int terrain_max_height = 50;

double pseudo_random(int x) {
	int seed = 0;
	int a = (seed + x) * 15485863;
	return 2 * (((a*a*a) % 2038074743) / 2038074743.0 - 0.5);
}

double noise(int x, int scale) {
	double p = x / (double)scale;
	
	double d1 = p - floor(p);
	double d2 = d1 - 1;

	double g1 = pseudo_random(floor(p));
	double g2 = pseudo_random(floor(p)+1);

	double v1 = g1 * d1;
	double v2 = g2 * d2;

	double amt = (1 - cos(d1*M_PI)) * 0.5;
	double val = amt * (v2 - v1) + v1;
	return val * 0.5 + 0.5;
}

double get_terrain_height(int x) {
	double combined_noise = noise(x, 128) * 0.6 +
		   					noise(x, 64) * 0.4;
	return combined_noise * terrain_max_height;
}

void render_terrain(Camera *camera) {
	for (int x = 0; x < camera->width; x++)
	{
		Vector2 point = {
			x, 0
		};
		point = get_world_coordinates(camera, point);
		point.y = floor(get_terrain_height(floor(point.x)) * PIXELS_PER_METER) / PIXELS_PER_METER;
		Vector2 render = get_screen_coordinates(camera, point);
		lineRGBA(camera->renderer, x, render.y, x, camera->height, 0xff, 0xff, 0xff, 0xff);
	}
}