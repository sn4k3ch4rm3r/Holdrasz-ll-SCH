#ifndef VECTOR_H
#define VECTOR_H

#include <SDL.h>

typedef struct Vector2 {
	double x;
	double y;
} Vector2;

Vector2 V_add(Vector2 v1, Vector2 v2);
Vector2 V_subtract(Vector2 v1, Vector2 v2);
Vector2 V_multiply_const(Vector2 v, double c);
Vector2 V_multiply(Vector2 v1, Vector2 v2);
Vector2 V_divide_const(Vector2 v, double c);
Vector2 V_normalize(Vector2 v);
Vector2 V_rotate(Vector2 v, double deg);
double V_len(Vector2 v);
double V_cross_product(Vector2 v1, Vector2 v2);
SDL_Point V_to_point(Vector2 v);

#endif
