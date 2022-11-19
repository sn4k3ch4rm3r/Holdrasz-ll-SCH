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

typedef struct ImpactPoint {
	Vector2 point;
	bool can_collide;
} ImpactPoint;

/// @brief Initializes the lander struct's data.
/// @param renderer Needed for loading sprites
/// @return Lander struct with default values set up
Lander init_lander(SDL_Renderer *renderer);

/// @brief Clean up memory after the game.
/// @param lander The lander struct used in the game
void destroy_lander(Lander *lander);

/// @brief Render the current frame of the lander.
/// @param camera Camera to render with 
/// @param lander Lander to render 
void render_lander(Camera *camera, Lander *lander);

void display_dashboard(Camera *camera, Lander *lander);

/// @brief Apply forces and update position and rotation of the lander.
/// @param lander The lander to update
/// @param dt Time since last frame
void update_lander(Lander *lander, double dt);

/// @brief Get the current inertia of the lander.
/// @param lander 
/// @return Current inertia based on the lander's mass
double get_lander_inertia(Lander *lander);

/// @brief Calculates the total mass based on remaining fuel
/// @param lander 
/// @return Total mass
double lander_total_mass(Lander *lander);

/// @brief Calculates the torque from a force applied to a given point on the lander.
/// @param point The point to apply the force to, in pixels relative to the top left corner of the sprite
/// @param force The force to apply
/// @return The resulting torque
double get_torque(Vector2 point, Vector2 force);

/// @brief Calculates the force applyed to the lander when colliding with the ground.
/// @param lander 
/// @param point Point of collision in pixels relative to the top left corner of the sprite
/// @param dt Time since the last frame
/// @return Force applied to the lander by the ground at the given point, {0, 0} if that point is not colliding
Vector2 get_impact_force(Lander *lander, Vector2 point, double dt);
#endif