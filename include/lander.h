#ifndef LANDER_H
#define LANDER_H

#include <SDL.h>
#include "vector.h"
#include "camera.h"

typedef struct Lander {
	Vector2 position;
	Vector2 velocity;
	double rotation;
	double angular_velocity;
	double dry_mass;
	double propellant;
} Lander;

void render_lander(Camera *camera, Lander *lander);
void lander_total_mass(Lander *lander);
#endif