#include <math.h>

double pseudo_random(int x) {
	int seed = 0;
	int a = (seed + x) * 15485863;
	return 2 * (((a*a*a) % 2038074743) / 2038074743.0 - 0.5);
}

double get_height(int x, int scale) {
	double p = x / (double)scale;
	
	double d1 = p - floor(p);
	double d2 = d1 - 1;

	double g1 = pseudo_random(floor(p));
	double g2 = pseudo_random(floor(p)+1);

	double v1 = g1 * d1;
	double v2 = g2 * d2;

	double amt = (1 - cos(d1*M_PI)) * 0.5;
	double val = amt * (v2 - v1) + v1;
	return val * 0.5 + 0.5;
}