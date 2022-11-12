#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <math.h>

#include "lander.h"
#include "vector.h"
#include "camera.h"
#include "terrain.h"

const int dry_mass = 7000;
const int propellant_mass = 8200;
const int inertia_min = 800000;
const int inertia_max = 1000000;
const int size_px = 64;
const int main_engine_thrust = 45040;
const int rcs_thrust = 20000;
const int main_engine_fuel_rate = 136;
const int rcs_fuel_rate = 13;
const double g = 1.62;
const double friction_coefficient = 0.5;
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
	//apply force
	Vector2 force = {0, 0};
	double torque = 0;
	if(lander->propellant > 0) {
		if(lander->engines[MAIN_ENGINE]) {
			force.y += main_engine_thrust;
			lander->propellant -= main_engine_fuel_rate * dt;
		}
		if(lander->engines[ROTATE_CW]) {
			Vector2 left_rcs = {20, 23};
			Vector2 right_rcs = {43, 14};

			Vector2 left_force = {0, rcs_thrust};
			Vector2 right_force = {0, -rcs_thrust};

			torque += get_torque(left_rcs, left_force);
			torque += get_torque(right_rcs, right_force);
			lander->propellant -= rcs_fuel_rate * dt;
		}
		if(lander->engines[ROTATE_CCW]) {
			Vector2 left_rcs = {20, 14};
			Vector2 right_rcs = {43, 23};

			Vector2 left_force = {0, -rcs_thrust};
			Vector2 right_force = {0, rcs_thrust};

			torque += get_torque(left_rcs, left_force);
			torque += get_torque(right_rcs, right_force);
			lander->propellant -= rcs_fuel_rate * dt;
		}
		if(lander->engines[LEFT_ENGINE]) {
			Vector2 rcs = {16, 19};
			Vector2 f = {rcs_thrust, 0};

			torque += get_torque(rcs, f);
			force = V_add(force, f);
			lander->propellant -= rcs_fuel_rate * dt;
		}
		if(lander->engines[RIGHT_ENGINE]) {
			Vector2 rcs = {47, 19};
			Vector2 f = {-rcs_thrust, 0};

			torque += get_torque(rcs, f);
			force = V_add(force, f);
			lander->propellant -= rcs_fuel_rate * dt;
		}
	}
	force = V_rotate(force, -lander->rotation);

	Vector2 left_leg = {2, 52};
	Vector2 right_leg = {62, 52};
	Vector2 impact_force;

	impact_force = get_impact_force(lander, left_leg, dt);
	force = V_add(force, impact_force);
	impact_force = V_rotate(impact_force, lander->rotation);
	torque += get_torque(left_leg, impact_force) * dt;

	impact_force = get_impact_force(lander, right_leg, dt);
	force = V_add(force, impact_force);
	impact_force = V_rotate(impact_force, lander->rotation);
	torque += get_torque(right_leg, impact_force) * dt;

	Vector2 accelartion = V_divide_const(force, lander_total_mass(lander));
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
	return inertia_min + (inertia_max - inertia_min) * (lander_total_mass(lander) / max_mass);
}

Vector2 to_metric(Vector2 point) {
	Vector2 metric = {
		point.x / PIXELS_PER_METER, 
		(size_px - point.y) / PIXELS_PER_METER
	};
	return metric;
}

double get_torque(Vector2 point, Vector2 force) {
	Vector2 r = V_subtract(to_metric(point), to_metric(center_of_mass));
	return V_cross_product(force, r);
}

Vector2 get_impact_force(Lander *lander, Vector2 point, double dt) {
	Vector2 center = {center_of_mass.x / PIXELS_PER_METER, -center_of_mass.y / PIXELS_PER_METER};
	point.y *= -1;
	point = V_divide_const(point, PIXELS_PER_METER);
	
	point = V_subtract(point, center);
	Vector2 velocity = V_add(lander->velocity, V_multiply_const(point, fabs(lander->angular_velocity)));

	point = V_rotate(point, -lander->rotation);
	point = V_add(V_add(point, center), lander->position);

	Vector2 force = {0, 0};
	if(point.y < get_terrain_height(point.x)) {
		force.y = lander_total_mass(lander) * (-velocity.y / dt);
		force.x = friction_coefficient * (lander_total_mass(lander) * (g - (velocity.y / dt)));
		if(lander->velocity.x > 0)
			force.x *= -1;
	}
	return force;
}
