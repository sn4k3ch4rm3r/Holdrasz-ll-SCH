#ifndef LANDER_H
#define LANDER_H

#include <SDL.h>
#include <stdbool.h>
#include "vector.h"
#include "camera.h"


typedef enum {
	NO_ENGINE = 0,
	MAIN_ENGINE = 1,
	LEFT_ENGINE = 2,
	RIGHT_ENGINE = 4,
	ROTATE_CW = 8,
	ROTATE_CCW = 16,
} Engine;

typedef struct Lander {
	Vector2 position;
	Vector2 velocity;
	double rotation;
	double angular_velocity;
	double dry_mass;
	double propellant;
	Engine engines;
} Lander;

Lander init_lander();
void render_lander(Camera *camera, Lander *lander);
void update_lander(Lander *lander, double dt);
void set_engine(Lander *lander, Engine engine, bool on);
double get_lander_inertia(Lander *lander);
double lander_total_mass(Lander *lander);
double get_torque(Vector2 point, Vector2 force);
#endif