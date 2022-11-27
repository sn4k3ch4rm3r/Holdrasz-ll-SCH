#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

#include "vector.h"

/// @brief The structure containing the camera
typedef struct Camera {
	Vector2 position;
	double zoom;
	SDL_Renderer *renderer;
	double width;
	double height;
} Camera;

/// @brief Used for converting between screen space and world space.
extern const double PIXELS_PER_METER;

/// @brief Updates the position and zoom level of the camera.
/// @param camera The camera struct to update
/// @param lander_pos Positoin of the lander for tracking
/// @param dt Time in seconds since the last frame
void update_camera(Camera *camera, Vector2 lander_pos, double dt);

/// @brief Convert world coordinates to screen coordinates.
/// @param camera The camera struct used for calculations
/// @param world_coordinates The point to be converted
/// @return The coordinates of the point in screen space
Vector2 get_screen_coordinates(Camera *camera, Vector2 world_coordinates);

/// @brief Convert screen coordinates to world coordinates.
/// @param camera The camera struct used for calculations
/// @param screen_coordinates the point to bo converted
/// @return The coordinates of the point in world space
Vector2 get_world_coordinates(Camera *camera, Vector2 screen_coordinates);

/// @brief Function to interpolate between two points
/// @param a point a
/// @param b point b
/// @param t time (0-1)
/// @return a if t = 0 b if t = 1
double lerp(double a, double b, double t);

#endif
