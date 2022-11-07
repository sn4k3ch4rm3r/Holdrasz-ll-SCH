#ifndef LANDER_H
#define LANDER_H

#include <SDL.h>
#include <stdbool.h>
#include "vector.h"
#include "camera.h"

typedef enum {
	MAIN_ENGINE,
	LEFT_ENGINE,
	RIGHT_ENGINE,
	ROTATE_CW,
	ROTATE_CCW,
} Engine;

typedef struct Lander {
	Vector2 position;
	Vector2 velocity;
	double rotation;
	double angular_velocity;
	double dry_mass;
	double propellant;
	bool engines[5];
	SDL_Texture *texture;
} Lander;

Lander init_lander(SDL_Renderer *renderer);
void destroy_lander(Lander *lander);
void render_lander(Camera *camera, Lander *lander);
void update_lander(Lander *lander, double dt);
double get_lander_inertia(Lander *lander);
double lander_total_mass(Lander *lander);
double get_torque(Vector2 point, Vector2 force);
#endif