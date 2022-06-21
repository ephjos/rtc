#pragma once

#include "canvas.h"
#include "matrix4.h"
#include "ray.h"
#include "world.h"

typedef struct camera_t {
	int hsize;
	int vsize;
	float fov;
	matrix4_t transform;
	float pixel_size;
	float half_width;
	float half_height;
} camera_t;

camera_t camera(int hsize, int vsize, float tfov);
ray_t camera_ray_for_pixel(camera_t c, int x, int y);
canvas_t camera_render(camera_t c, world_t w);

void camera_demo();

