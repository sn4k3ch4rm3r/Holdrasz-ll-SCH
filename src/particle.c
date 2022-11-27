#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>

#include "particle.h"
#include "vector.h"
#include "camera.h"

#include "debugmalloc.h"

void append_particle(List *list, Particle p) {
	ListElement *element = malloc(sizeof(ListElement));

	element->particle = p;
	element->prev = list->last;
	element->next = NULL;

	if(list->last != NULL) {
		list->last->next = element;
	}

	list->last = element;
	if(list->first == NULL) {
		list->first = element;
	}
}

void update_particles(List *list, double dt) {
	ListElement *element = list->first;
	while(element != NULL) {
		element->particle.position = V_add(element->particle.position, V_multiply_const(element->particle.velocity, dt));
		element->particle.lived += dt;
		ListElement *next = element->next;
		if(element->particle.lived >= element->particle.life_time) {
			delete_particle(list, element);
		}
		element = next;
	}
}

void render_particles(Camera *camera, List *list) {
	for (ListElement *element = list->first; element != NULL; element = element->next) {
		Vector2 screen_coords = get_screen_coordinates(camera, element->particle.position);
		SDL_Color color = {
			lerp(element->particle.start_color.r, element->particle.end_color.r, element->particle.lived / element->particle.life_time),
			lerp(element->particle.start_color.g, element->particle.end_color.g, element->particle.lived / element->particle.life_time),
			lerp(element->particle.start_color.b, element->particle.end_color.b, element->particle.lived / element->particle.life_time),
			lerp(element->particle.start_color.a, element->particle.end_color.a, element->particle.lived / element->particle.life_time)
		};
		filledCircleRGBA(camera->renderer, screen_coords.x, screen_coords.y, element->particle.size * (1 - element->particle.lived / element->particle.life_time) * camera->zoom, color.r, color.g, color.b, color.a);
	}
}

void delete_particle(List *list, ListElement *particle) {
	if(list->first == particle) {
		list->first = particle->next;
	}
	else{
		particle->prev->next = particle->next;
	}

	if(list->last == particle) {
		list->last = particle->prev;
	}
	else {
		particle->next->prev = particle->prev;
	}

	free(particle);
}

void destroy_particles(List *list) {
	ListElement *current = list->first;
	while(current != NULL) {
		ListElement *next = current->next;
		free(current);
		current = next;
	}
}