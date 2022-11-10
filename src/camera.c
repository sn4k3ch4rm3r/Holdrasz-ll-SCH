#include "game.h"
#include "vector.h"

const double pixels_per_meter = 7;
const int camera_speed = 5;

double lerp(double a, double b, double t) {
	return a * (1-t) + b * t;
}

Vector2 lerp2(Vector2 a, Vector2 b, double t) {
	return V_add(V_multiply_const(a, 1-t), V_multiply_const(b, t));
}

void update_camera(Camera *camera, Vector2 lander_pos, double dt) {
	Vector2 center = {
		camera->width/2 - 32,
		camera->height/2 - 200
	};

	Vector2 target_pos = V_add(camera->position, V_subtract(lander_pos, get_world_coordinates(camera, center)));

	double min_height = camera->height / (pixels_per_meter * camera->zoom);
	if(target_pos.y < min_height) {
		target_pos.y = min_height;
	}

	camera->position = lerp2(camera->position, target_pos, camera_speed * dt);
	
	double target_zoom = 0.5;
	if(lander_pos.y < 50) {
		target_zoom = 2;
	}
	else if(lander_pos.y < 100) {
		target_zoom = 1;
	}
	camera->zoom = lerp(camera->zoom, target_zoom, camera_speed * dt);
}

Vector2 get_world_coordinates(Camera *camera, Vector2 screen_coordinates) {
	Vector2 world_coordinates = V_divide_const(screen_coordinates, pixels_per_meter * camera->zoom);
	world_coordinates.y = -world_coordinates.y;
	world_coordinates = V_add(world_coordinates, camera->position);
	return world_coordinates;
}

//wc - cc = sc
Vector2 get_screen_coordinates(Camera *camera, Vector2 world_coordinates) {
	Vector2 screen_coordinates = V_subtract(world_coordinates, camera->position);
	screen_coordinates = V_multiply_const(screen_coordinates, camera->zoom * pixels_per_meter);
	screen_coordinates.y *= -1;
	
	return screen_coordinates;
}