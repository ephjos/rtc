#pragma once

#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions

#define ERROR_WRITE_PPM_HEADER 10
#define ERROR_CANVAS_DEMO_FILE_OPEN 11
#define ERROR_MATRIX_NONINVERTIBLE 12
#define ERROR_GROW_INTERSECTION_LIST 13
#define ERROR_GROW_WORLD_POINT_LIGHTS_LIST 14
#define ERROR_GROW_WORLD_SHAPES_LIST 15

typedef struct camera_t camera_t;
typedef struct canvas_t canvas_t;
typedef struct color_t color_t;
typedef struct computations_t computations_t;
typedef struct intersection_t intersection_t;
typedef struct intersection_list_t intersection_list_t;
typedef struct point_light_t point_light_t;
typedef struct material_t material_t;
typedef struct matrix4_t matrix4_t;
typedef struct ray_t ray_t;
typedef struct shape_t shape_t;
typedef struct sphere_t sphere_t;
typedef struct vec4_t vec4_t;
typedef struct world_t world_t;

////////////////////////////////////////////////////////////////////////////////
// Vec4

typedef struct vec4_t {
	double x;
	double y;
	double z;
	double w;
} vec4_t;

static const vec4_t VEC4_ZERO = {0, 0, 0, 0};

void vec4_print(vec4_t v);

vec4_t tuple(double x, double y, double z, double w);
vec4_t point(double x, double y, double z);
vec4_t vector(double x, double y, double z);
bool vec4_is_point(vec4_t v);
bool vec4_is_vector(vec4_t v);
bool vec4_eq(vec4_t a, vec4_t b);

vec4_t vec4_add(vec4_t a, vec4_t b);
vec4_t vec4_sub(vec4_t a, vec4_t b);
vec4_t vec4_neg(vec4_t a);
vec4_t vec4_muls(vec4_t a, double s);
vec4_t vec4_divs(vec4_t a, double s);

double vec4_magnitude(vec4_t v);
vec4_t vec4_normalize(vec4_t v);
double vec4_dot(vec4_t a, vec4_t b);
vec4_t vec4_cross(vec4_t a, vec4_t b);
vec4_t vec4_reflect(vec4_t in, vec4_t normal);

void vec4_demo();

////////////////////////////////////////////////////////////////////////////////
// Matrix4

typedef struct matrix4_t {
	double _00; double _01; double _02; double _03;
	double _10; double _11; double _12; double _13;
	double _20; double _21; double _22; double _23;
	double _30; double _31; double _32; double _33;
} matrix4_t;

matrix4_t matrix4(
		double _00, double _01, double _02, double _03,
		double _10, double _11, double _12, double _13,
		double _20, double _21, double _22, double _23,
		double _30, double _31, double _32, double _33
		);
matrix4_t matrix4_identity();

bool matrix4_eq(matrix4_t a, matrix4_t b);

matrix4_t matrix4_mul(matrix4_t a, matrix4_t b);
vec4_t matrix4_mul_vec4(matrix4_t a, vec4_t b);
matrix4_t matrix4_transpose(matrix4_t a);
double matrix4_determinant(matrix4_t M);
bool matrix4_is_invertible(matrix4_t a);
matrix4_t matrix4_inverse(matrix4_t M);


////////////////////////////////////////////////////////////////////////////////
// Camera

typedef struct camera_t {
	int hsize;
	int vsize;
	double fov;
	matrix4_t transform;
	double pixel_size;
	double half_width;
	double half_height;
} camera_t;

camera_t camera(int hsize, int vsize, double tfov);
ray_t camera_ray_for_pixel(camera_t c, int x, int y);
canvas_t camera_render(camera_t c, world_t w);

void camera_demo();
void plane_demo();


////////////////////////////////////////////////////////////////////////////////
// Canvas

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
void canvas_save(canvas_t c, char *filename);

////////////////////////////////////////////////////////////////////////////////
// Color

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

////////////////////////////////////////////////////////////////////////////////
// Lights

typedef struct point_light_t {
	vec4_t position;
	color_t intensity;
} point_light_t;

point_light_t point_light(vec4_t position, color_t intensity);
bool point_light_eq(point_light_t a, point_light_t b);

////////////////////////////////////////////////////////////////////////////////
// Materials

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

////////////////////////////////////////////////////////////////////////////////
// Plane

typedef struct plane_t {
} plane_t;

void plane_intersect(shape_t s, ray_t r, intersection_list_t *ilist);
vec4_t plane_normal_at(shape_t s, vec4_t p);
shape_t plane();

////////////////////////////////////////////////////////////////////////////////
// Ray

typedef struct ray_t {
  vec4_t origin;
  vec4_t direction;
} ray_t;

ray_t ray(vec4_t origin, vec4_t direction);
vec4_t ray_position(ray_t r, double t);
ray_t ray_transform(ray_t r, matrix4_t m);

void ray_demo();

////////////////////////////////////////////////////////////////////////////////
// Shape

typedef struct shape_t {
	matrix4_t transform;
	matrix4_t inverse_transform;
	material_t material;
	void *instance;
	void (*intersect)(shape_t s, ray_t r, intersection_list_t *ilist);
	vec4_t (*normal_at)(shape_t s, vec4_t p);
	bool (*eq)(shape_t a, shape_t b);
} shape_t;

bool shape_eq(shape_t a, shape_t b);
void shape_set_transform(shape_t *s, matrix4_t t);
void shape_free(shape_t s);

////////////////////////////////////////////////////////////////////////////////
// Sphere

typedef struct sphere_t {
} sphere_t;

void sphere_intersect(shape_t s, ray_t r, intersection_list_t *ilist);
vec4_t sphere_normal_at(shape_t s, vec4_t p);
shape_t sphere();

////////////////////////////////////////////////////////////////////////////////
// Computations

typedef struct computations_t {
	double t;
	shape_t s;
	vec4_t point;
	vec4_t eyev;
	vec4_t normalv;
	bool inside;
	vec4_t over_point;
} computations_t;

computations_t prepare_computations(intersection_t i, ray_t r);

////////////////////////////////////////////////////////////////////////////////
// Intersections
typedef struct intersection_t {
	double t;
	shape_t s;
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

void intersection_list_free(intersection_list_t ilist);
void add_intersection(intersection_list_t *ilist, intersection_t item);
intersection_t* hit(intersection_list_t ilist);

////////////////////////////////////////////////////////////////////////////////
// Transform

matrix4_t translation(double x, double y, double z);
matrix4_t scaling(double x, double y, double z);
matrix4_t rotate_x(double r);
matrix4_t rotate_y(double r);
matrix4_t rotate_z(double r);
matrix4_t shearing(double a, double b, double c, double d, double e, double f);

matrix4_t transform(matrix4_t *ms, int n);

matrix4_t view_transform(vec4_t from, vec4_t to, vec4_t up);

void transform_demo();

////////////////////////////////////////////////////////////////////////////////
// Util

bool req(double a, double b);

#define EPSILON (0.00001)

#undef MAX
#define MAX( a, b ) ((a) > (b) ? (a) : (b))

#undef MIN
#define MIN( a, b ) ((a) > (b) ? (b) : (a))

#undef CLAMP
#define CLAMP( x, a, b ) (MIN(MAX(x, a), b))


////////////////////////////////////////////////////////////////////////////////
// World

typedef struct world_t {
	int n_point_lights;
	int cap_point_lights;
	point_light_t *point_lights;

	int n_shapes;
	int cap_shapes;
	shape_t *shapes;
} world_t;

world_t world();
void world_add_point_light(world_t *w, point_light_t pl);
void world_add_shape(world_t *w, shape_t s);
void world_free(world_t w);

world_t default_world();

intersection_list_t world_intersect_ray(world_t w, ray_t r);
color_t world_shade_hit(world_t w, computations_t c);
color_t world_color_at(world_t w, ray_t r);
bool world_point_is_shadowed(world_t w, vec4_t p, int point_lights_i);
