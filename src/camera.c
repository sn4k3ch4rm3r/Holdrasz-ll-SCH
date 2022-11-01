#include "camera.h"
#include "vector.h"

const double pixels_per_meter = 7;

double get_camera_height(Camera *camera) {
	return camera->position.y > camera->height ? camera->position.y : camera->height;
}

Vector2 get_screen_coordinates(Camera *camera, Vector2 world_coordinates) {
	Vector2 screen_coordinates = V_subtract(world_coordinates, camera->position);
	screen_coordinates = V_multiply_const(screen_coordinates, camera->zoom * pixels_per_meter);
	screen_coordinates.y = camera->height - screen_coordinates.y;
	
	return screen_coordinates;
}