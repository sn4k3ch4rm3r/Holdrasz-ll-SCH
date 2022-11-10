#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <math.h>

#include "lander.h"
#include "vector.h"
#include "camera.h"

const int dry_mass = 7000;
const int propellant_mass = 8200;
const int inertia_min = 800000;
const int inertia_max = 1000000;
const int size_px = 64;
const int main_engine_thrust = 45040;
const int rcs_thrust = 20000;
const int main_engine_fuel_rate = 136;
const int rcs_fuel_rate = 13; 
const Vector2 center_of_mass = {32.5, 20};

Lander init_lander(SDL_Renderer *renderer) {
	Lander lander = {
		{0, 60},
		{0, 0},
		-90,
		0,
		dry_mass,
		propellant_mass,
		{0},
		IMG_LoadTexture(renderer, "assets/lunar_module.png")
	};
	return lander;
}

void destroy_lander(Lander *lander) {
	SDL_DestroyTexture(lander->texture);
}

void render_lander(Camera *camera, Lander *lander) {
	Vector2 screen_coords = get_screen_coordinates(camera, lander->position);
	SDL_Point center = V_to_point(V_multiply_const(center_of_mass, camera->zoom));
	SDL_Rect src = {0, 0, size_px, size_px};
	SDL_Rect dst = {screen_coords.x, screen_coords.y, size_px * camera->zoom, size_px * camera->zoom};

	// -1 is the lander itself
	int render_order[] = {MAIN_ENGINE, -1, LEFT_ENGINE, RIGHT_ENGINE, ROTATE_CW, ROTATE_CCW};
	for (int i = 0; i < 6; i++) {
		if(render_order[i] == -1 || (lander->engines[render_order[i]] && lander->propellant > 0)) {
			src.x = (render_order[i] + 1) * size_px;
			SDL_RenderCopyEx(camera->renderer, lander->texture, &src, &dst, lander->rotation, &center, SDL_FLIP_NONE);
		}
	}
}

void update_lander(Lander *lander, double dt) {
	const double g = 1.62;

	//apply force
	Vector2 force = {0, 0};
	double torque = 0;
	if(lander->propellant > 0) {
		if(lander->engines[MAIN_ENGINE]) {
			force.y += main_engine_thrust;
			lander->propellant -= main_engine_fuel_rate * dt;
		}
		if(lander->engines[ROTATE_CW]) {
			Vector2 left_rcs = {2.79, 5.86};
			Vector2 right_rcs = {6.21, 6.86};

			Vector2 left_force = {0, rcs_thrust};
			Vector2 right_force = {0, -rcs_thrust};

			torque += get_torque(left_rcs, left_force);
			torque += get_torque(right_rcs, right_force);
			lander->propellant -= rcs_fuel_rate * dt;
		}
		if(lander->engines[ROTATE_CCW]) {
			Vector2 left_rcs = {2.79, 6.86};
			Vector2 right_rcs = {6.21, 5.86};

			Vector2 left_force = {0, -rcs_thrust};
			Vector2 right_force = {0, rcs_thrust};

			torque += get_torque(left_rcs, left_force);
			torque += get_torque(right_rcs, right_force);
			lander->propellant -= rcs_fuel_rate * dt;
		}
		if(lander->engines[LEFT_ENGINE]) {
			Vector2 rcs = {2.29, 6.36};
			Vector2 f = {rcs_thrust, 0};

			torque += get_torque(rcs, f);
			force = V_add(force, f);
			lander->propellant -= rcs_fuel_rate * dt;
		}
		if(lander->engines[RIGHT_ENGINE]) {
			Vector2 rcs = {6.71, 6.36};
			Vector2 f = {-rcs_thrust, 0};

			torque += get_torque(rcs, f);
			force = V_add(force, f);
			lander->propellant -= rcs_fuel_rate * dt;
		}
	}

	Vector2 rotated_force;
	double rotation_rad = -lander->rotation * M_PI / 180;
	rotated_force.x = force.x * cos(rotation_rad) - force.y * sin(rotation_rad);
	rotated_force.y = force.x * sin(rotation_rad) + force.y * cos(rotation_rad);

	Vector2 accelartion = V_divide_const(rotated_force, lander_total_mass(lander));
	accelartion.y -= g;
	double angular_acceleration = (torque / get_lander_inertia(lander)) / M_PI * 180;

	//update velocity
	lander->velocity = V_add(lander->velocity, V_multiply_const(accelartion, dt));
	lander->angular_velocity += angular_acceleration * dt;

	//update position
	lander->position = V_add(lander->position, V_multiply_const(lander->velocity, dt));
	lander->rotation += lander->angular_velocity;
}

double lander_total_mass(Lander *lander) {
	return lander->dry_mass + lander->propellant;
}

double get_lander_inertia(Lander *lander) {
	double max_mass = dry_mass + propellant_mass;
	double current_mass = lander_total_mass(lander);
	return inertia_min + (inertia_max - inertia_min) * (current_mass / max_mass);
}

double get_torque(Vector2 point, Vector2 force) {
	Vector2 center_metric = {center_of_mass.x / 7, (size_px - center_of_mass.y) / 7};
	Vector2 r = V_subtract(point, center_metric);
	return V_cross_product(force, r);
}
