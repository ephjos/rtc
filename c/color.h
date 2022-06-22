#pragma once

typedef struct color_t {
	double r;
	double g;
	double b;
} color_t;

color_t color(double r, double g, double b);
bool color_eq(color_t a, color_t b);

color_t color_add(color_t a, color_t b);
color_t color_sub(color_t a, color_t b);
color_t color_muls(color_t a, double s);

// Hadamard Product
color_t color_mul(color_t a, color_t b);
