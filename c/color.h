#pragma once

typedef struct color_t {
	float r;
	float g;
	float b;
} color_t;

color_t color(float r, float g, float b);
bool color_eq(color_t a, color_t b);

color_t color_add(color_t a, color_t b);
color_t color_sub(color_t a, color_t b);
color_t color_muls(color_t a, float s);

// Hadamard Product
color_t color_mul(color_t a, color_t b);
