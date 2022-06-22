#pragma once

#include <stdbool.h>

#include "canvas.h"
#include "lights.h"
#include "vec4.h"

typedef struct material_t {
	color_t color;
	double ambient;
	double diffuse;
	double specular;
	double shininess;
} material_t;

material_t material_default();
bool material_eq(material_t a, material_t b);

color_t lighting(
		material_t m,
		point_light_t light,
		vec4_t point,
		vec4_t eyev,
		vec4_t normalv,
		bool in_shadow
		);

void material_demo();
