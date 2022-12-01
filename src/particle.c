#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <math.h>

#include "particle.h"
#include "vector.h"
#include "camera.h"
#include "terrain.h"

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
		double ds = 0.5;
		Vector2 p1 = {
			floor(element->particle.position.x), get_terrain_height(floor(element->particle.position.x))
		};
		Vector2 p2 = {
			floor(element->particle.position.x)+ds, get_terrain_height(floor(element->particle.position.x)+ds)
		};

		double slope = p2.y - p1.y;
		double angle = (atan(slope/ds) / M_PI) * 180;

		Vector2 force = {0, 0};
		if(element->particle.position.y < get_terrain_height(element->particle.position.x)) {
			Vector2 v = V_rotate(element->particle.velocity, -angle);
			force.y = 0.01 * 2 * (-v.y / dt);
			force.x = 0.01 * 2 * (-v.x / dt);
			double impact_angle = tan(force.y / force.x) / M_PI * 180;
			force = V_rotate(force, angle + 180 - impact_angle);
		}
		Vector2 acceleration = V_divide_const(force, .01);
		element->particle.velocity = V_add(element->particle.velocity, V_multiply_const(acceleration, dt));

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
	list->first = NULL;
}