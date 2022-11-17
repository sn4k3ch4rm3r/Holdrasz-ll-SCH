#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

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
const double friction_coefficient = 0.2;
const Vector2 center_of_mass = {32.5, 20};

SDL_Texture *lander_texture;
SDL_Texture *dashboard_texture;

Lander init_lander(SDL_Renderer *renderer) {
	Lander lander = {
		{0, 200},
		{100, 0},
		-90,
		0,
		dry_mass,
		propellant_mass,
		{0},
		IMG_LoadTexture(renderer, "assets/lunar_module.png")
	};
	lander_texture = IMG_LoadTexture(renderer, "assets/lunar_module.png");
	dashboard_texture = IMG_LoadTexture(renderer, "assets/dashboard.png");
	return lander;
}

void destroy_lander(Lander *lander) {
	SDL_DestroyTexture(lander->texture);
	SDL_DestroyTexture(lander_texture);
	SDL_DestroyTexture(dashboard_texture);
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

void display_dashboard(Camera *camera, Lander *lander) {
	//Render the backgound
	SDL_Rect base_rect = {0, 0, 211, 128};
	SDL_Rect dashboard_rect = {0, 0, 422, 256};
	SDL_RenderCopy(camera->renderer, dashboard_texture, &base_rect, &dashboard_rect);
	
	//Display current altitude
	char alt_str[10];
	snprintf(alt_str, 10, "%.1lf m", lander->position.y);
	stringRGBA(camera->renderer, 20, 204, alt_str, 0xff, 0xff, 0xff, 0xff);

	//Display fuel remaining
	if(lander->propellant > 0){
		int fuel_height = 147 - ((147 - 14) * (lander->propellant / propellant_mass));
		thickLineRGBA(camera->renderer, 38, 147, 38, fuel_height, 44, 0x75, 0xa4, 0x43, 0xff);
	}

	int handle_len = 50;
	Vector2 handle_center = {
		153,
		67
	};
	Vector2 handle_pos;
	//Display Vy
	handle_pos.x = sin(lander->velocity.y / 20 * M_PI_2) * handle_len;
	handle_pos.y = -cos(lander->velocity.y / 20 * M_PI_2) * handle_len;
	handle_pos = V_add(handle_center, handle_pos);
	thickLineRGBA(camera->renderer, handle_center.x, handle_center.y, handle_pos.x, handle_pos.y, 2, 0xa5, 0x30, 0x30, 0xff);
	
	//Display Vx
	handle_center.y = 185;
	handle_pos.x = sin(lander->velocity.x / 100 * M_PI_2) * handle_len;
	handle_pos.y = -cos(lander->velocity.x / 100 * M_PI_2) * handle_len;
	handle_pos = V_add(handle_center, handle_pos);
	thickLineRGBA(camera->renderer, handle_center.x, handle_center.y, handle_pos.x, handle_pos.y, 2, 0xa5, 0x30, 0x30, 0xff);

	//Display indicator lights
	SDL_Rect indicator_light = {211, 80, 14, 14};
	SDL_Rect light_dst = {0, 192, 28, 28};
	if(fabs(lander->velocity.x) < 2) {
		light_dst.x = 234;
		SDL_RenderCopy(camera->renderer, dashboard_texture, &indicator_light, &light_dst);
	}
	if(fabs(lander->velocity.y) < 2) {
		light_dst.x = 282;
		SDL_RenderCopy(camera->renderer, dashboard_texture, &indicator_light, &light_dst);
	}
	if(abs(lander->rotation) % 360 < 10 || abs(lander->rotation) % 360 > 350) {
		light_dst.x = 330;
		SDL_RenderCopy(camera->renderer, dashboard_texture, &indicator_light, &light_dst);
	}
	///@TODO: Display contact indicator

	//Display artificial horizon
	SDL_Rect horizon_rect = {211, 0, 74, 74};
	SDL_Rect horizon_dst = {248, 16, 144, 144};
	SDL_RenderCopyEx(camera->renderer, dashboard_texture, &horizon_rect, &horizon_dst, lander->rotation, NULL, SDL_FLIP_NONE);
	thickLineRGBA(camera->renderer, 230, 88, 270, 88, 4, 0xde, 0x9e, 0x41, 0xff);
	thickLineRGBA(camera->renderer, 368, 88, 408, 88, 4, 0xde, 0x9e, 0x41, 0xff);
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

	const double ds = 0.5;

	Vector2 p1 = {
		floor(point.x), get_terrain_height(floor(point.x))
	};
	Vector2 p2 = {
		floor(point.x)+ds, get_terrain_height(floor(point.x)+ds)
	};

	double slope = p2.y - p1.y;
	double angle = (atan(slope/ds) / M_PI) * 180;

	Vector2 force = {0, 0};
	if(point.y < get_terrain_height(point.x)) {
		velocity = V_rotate(velocity, -angle);
		force.y = lander_total_mass(lander) * (-velocity.y / dt);
		force.x = friction_coefficient * (lander_total_mass(lander) * (g - velocity.y / dt));
		if(lander->velocity.x > 0) {
			force.x *= -1;
		}
		force = V_rotate(force, angle);
	}
	return force;
}
