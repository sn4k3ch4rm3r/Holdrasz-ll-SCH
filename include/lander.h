#ifndef LANDER_H
#define LANDER_H

#include <SDL.h>
#include "vector.h"

typedef struct Lander {
	Vector2 pos;
	Vector2 velocity;
	double rotation;
	double angular_velocity;
	double dry_mass;
	double propellant;
} Lander;

void render_lander(SDL_Renderer *renderer, Lander *lander);
void lander_total_mass(Lander *lander);
#endif