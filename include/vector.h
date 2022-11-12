#ifndef VECTOR_H
#define VECTOR_H

#include <SDL.h>

typedef struct Vector2 {
	double x;
	double y;
} Vector2;

/// @brief Adds two vectors together.
Vector2 V_add(Vector2 v1, Vector2 v2);

/// @brief Suptracts two vectors from each other.
Vector2 V_subtract(Vector2 v1, Vector2 v2);

/// @brief Multiplies a vector by a constant value.
Vector2 V_multiply_const(Vector2 v, double c);

/// @brief Multiplies two vectors together.
Vector2 V_multiply(Vector2 v1, Vector2 v2);

/// @brief Divides a vector by a constant value.
Vector2 V_divide_const(Vector2 v, double c);

/// @brief Calculates the vector with the same direction as the original, but with a length of 1.
Vector2 V_normalize(Vector2 v);

/// @brief Rotates the vector.
/// @param v 
/// @param deg Amount to rotate by in degrees 
Vector2 V_rotate(Vector2 v, double deg);

/// @brief Calculates the length of a vector.
double V_len(Vector2 v);

/// @brief Calculates the cross product of two vectors 
double V_cross_product(Vector2 v1, Vector2 v2);

/// @brief Converts a Vector2 to an SDL_Point 
SDL_Point V_to_point(Vector2 v);

#endif
