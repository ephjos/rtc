
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "canvas.h"
#include "color.h"
#include "error.h"
#include "util.h"
#include "vec4.h"

canvas_t canvas(int width, int height) {
	canvas_t c;
	c.width = width;
	c.height = height;

	// calloc initializes all bytes in the range to 0, which makes the
	// default pixel color black
	c.data = (color_t*)calloc(width*height, sizeof(color_t));
	return c;
}

void canvas_write(canvas_t *c, int y, int x, color_t p) {
	c->data[(x*c->width)+y] = p;
}

color_t canvas_get(canvas_t c, int y, int x) {
	return c.data[(x*c.width)+y];
}


void canvas_free(canvas_t c) {
	free(c.data);
}

char* canvas_to_ppm(canvas_t c) {
	int byte_count = 0;
	byte_count += 3; // magic
	byte_count += 2 + (int)log10(c.width) + 1 + (int)log10(c.height) + 1; // dimensions
	byte_count += 4; // color
	int header_bytes = byte_count+1;
	byte_count += (c.width*c.height) * 12; // safe assumes 4 bytes per channel (will overallocate)
	byte_count += 1; // ensure null terminator

	char *ppm = (char*)calloc(byte_count, sizeof(char));
	int n = snprintf(ppm, header_bytes, "P3\n%d %d\n255\n", c.width, c.height);
	if (n != header_bytes-1) {
		fprintf(stderr, "Unable to write PPM header, expected %d bytes but wrote %d\n", header_bytes-1, n);
		exit(ERROR_WRITE_PPM_HEADER);
	}

	for (int i = 0; i < c.height; i++) {
		for (int j = 0; j < c.width; j++) {
			color_t color = canvas_get(c, j, i);
			int r = CLAMP(color.r*255.0, 0, 255);
			int g = CLAMP(color.g*255.0, 0, 255);
			int b = CLAMP(color.b*255.0, 0, 255);

			n += sprintf(ppm+n, "%d %d %d ", r, g, b);
		}

		n += sprintf(ppm+n, "\n");
	}

	return ppm;
}

void canvas_save(canvas_t c, char *filename) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(ERROR_CANVAS_DEMO_FILE_OPEN);
	}

	char *ppm = canvas_to_ppm(c);
	fprintf(fp, "%s", ppm);
	free(ppm);
	fclose(fp);
}


TEST_CASE(creating_a_canvas) {
	canvas_t c = canvas(10, 20);
	ASSERT_TRUE(req(c.width, 10));
	ASSERT_TRUE(req(c.height, 20));
	for (int i = 0; i < c.height; i++) {
		for (int j = 0; j < c.width; j++) {
			ASSERT_TRUE(color_eq(canvas_get(c, i, j), color(0,0,0)));
		}
	}
}

TEST_CASE(writing_pixels_to_a_canvas) {
	canvas_t c = canvas(10, 20);
	color_t red = color(1, 0, 0);
	canvas_write(&c, 2, 3, red);
	ASSERT_TRUE(color_eq(canvas_get(c, 2, 3), red));
}

TEST_CASE(constructing_the_ppm_header) {
	canvas_t c = canvas(5, 3);
	char *ppm = canvas_to_ppm(c);
	ASSERT_TRUE(strncmp(ppm, "P3\n5 3\n255\n", 11) == 0);
}

TEST_CASE(constructing_ppm_pixel_data) {
	canvas_t c = canvas(5, 3);
	color_t c1 = color(1.5, 0, 0);
	color_t c2 = color(0, 0.5, 0);
	color_t c3 = color(-0.5, 0, 1);
	canvas_write(&c, 0, 0, c1);
	canvas_write(&c, 2, 1, c2);
	canvas_write(&c, 4, 2, c3);
	char *ppm = canvas_to_ppm(c);
	ASSERT_TRUE(strcmp(ppm, "P3\n5 3\n255\n255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 127 0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 \n") == 0);
}

void canvas_demo() {
	typedef struct env_t {
		vec4_t gravity;
		vec4_t wind;
	} env_t;
	typedef struct proj_t {
		vec4_t position;
		vec4_t velocity;
	} proj_t;

	proj_t tick(env_t e, proj_t p) {
		proj_t proj;
		proj.position = vec4_add(p.position, p.velocity);
		proj.velocity = vec4_add(vec4_add(p.velocity, e.gravity), e.wind);
		return proj;
	}

	void canvas_write_proj(canvas_t* c, int x, int y, color_t p) {
		const int w = 5;
		for (int i = -w; i <= w; i++) {
			for (int j = -w; j <= w; j++) {
				int ii = x-i;
				int jj = y-j;
				if (ii >= 0 && ii < c->width && jj >= 0 && jj < c->height) {
					canvas_write(c, ii, jj, p);
				}
			}
		}
	}

	proj_t p;
	p.position = point(0, 1, 0);
	p.velocity = vec4_muls(vec4_normalize(vector(1, 1.8, 0)), 11.25);

	env_t e;
	e.gravity = vector(0, -0.1, 0);
	e.wind = vector(-0.01, 0, 0);

	canvas_t c = canvas(900, 550);
	color_t red = color(1, 0, 0);

	while (p.position.y > 0) {
		canvas_write_proj(&c, p.position.x, 550-p.position.y, red);
		p = tick(e, p);
	}

	canvas_write_proj(&c, MIN(p.position.x, 899), 549, red);

	canvas_save(c, "./canvas_demo.ppm");
	canvas_free(c);
}
