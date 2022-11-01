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

double get_camera_height(Camera *camera);
Vector2 get_screen_coordinates(Camera *camera, Vector2 world_coordinates);
#endif
