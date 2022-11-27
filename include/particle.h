#ifndef LIST_H
#define LIST_H

#include "vector.h"
#include "camera.h"

typedef struct Particle {
	SDL_Color start_color;
	SDL_Color end_color;
	Vector2 velocity;
	Vector2 position;
	double life_time;
	double lived;
} Particle;

typedef struct ListElement ListElement;

struct ListElement {
	Particle particle;
	ListElement *next;
	ListElement *prev;
};

typedef struct List {
	ListElement *first;
	ListElement *last;
} List;


void append_particle(List *list, Particle p);
void delete_particle(List *list, ListElement *particle);
void update_particles(List *list, double dt);
void render_particles(Camera *camera, List *list);
void destroy_particles(List *list);

#endif