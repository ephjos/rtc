#pragma once

typedef struct intersection_t {
	float t;
	int type; // The type of the object hit
	int id;   // The of the individial object
	// Combine type and id to lookup the object
} intersection_t;

typedef struct intersection_list_t {
	int size;
	int cap;
	intersection_t *items;
	intersection_t *hit;
} intersection_list_t;

#define INTERSECTION_LIST_INITIAL_CAP 16

// Caller responsible for freeing
intersection_list_t intersection_list();

void free_intersection_list(intersection_list_t ilist);
void add_intersection(intersection_list_t *ilist, intersection_t item);
intersection_t* hit(intersection_list_t ilist);
