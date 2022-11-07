#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

#include "vector.h"

typedef struct Camera {
	Vector2 position;
	double zoom;
	SDL_Renderer *renderer;
	double width;
	double height;
} Camera;

void update_camera(Camera *camera, Vector2 lander_pos, double dt);
Vector2 get_screen_coordinates(Camera *camera, Vector2 world_coordinates);
Vector2 get_world_coordinates(Camera *camera, Vector2 screen_coordinates);
#endif
