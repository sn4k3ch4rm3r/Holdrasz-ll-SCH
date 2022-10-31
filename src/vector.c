#include <math.h>
#include "vector.h"

Vector2 V_add(Vector2 v1, Vector2 v2) {
	Vector2 res;
	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	return res;
}

Vector2 V_subtract(Vector2 v1, Vector2 v2) {
	Vector2 res;
	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	return res;
}

Vector2 V_multiply_const(Vector2 v, double c) {
	Vector2 res;
	res.x = v.x * c;
	res.y = v.y * c;
	return res;
}

Vector2 V_multiply(Vector2 v1, Vector2 v2) {
	Vector2 res;
	res.x = v1.x * v2.x;
	res.y = v1.y * v2.y;
	return res;
}

Vector2 V_divide_const(Vector2 v, double c) {
	return V_multiply_const(v, 1/c);
}

double V_len(Vector2 v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

Vector2 V_normalize(Vector2 v) {
	Vector2 res;
	double len = V_len(v);
	res.x = v.x / len;
	res.y = v.y / len;
	return res;
}