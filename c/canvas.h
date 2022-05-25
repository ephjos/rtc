#pragma once

#include "color.h"

typedef struct canvas_t {
	int width;
	int height;
	color_t *data;
} canvas_t;

canvas_t canvas(int width, int height);

void canvas_write(canvas_t *c, int x, int y, color_t p);
color_t canvas_get(canvas_t c, int x, int y);

void canvas_free(canvas_t c);

// Caller responsible for freeing
char* canvas_to_ppm(canvas_t c);

void canvas_demo();
