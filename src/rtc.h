#ifndef __RTC_H__
#define __RTC_H__

// Headers
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


// Numeric types
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef unsigned long long int u64;

typedef signed char s8;
typedef signed short int s16;
typedef signed long int s32;
typedef signed long long int s64;

typedef s32 b32;

typedef float f32;
typedef double f64;

// Constants
#define PI           3.14159265358979323846
#define PI_2         (PI / 2.0)
#define PI_3         (PI / 3.0)
#define PI_4         (PI / 4.0)
#define PI_5         (PI / 5.0)
#define PI_6         (PI / 6.0)

#define ROOT_2       1.41421356237
#define ROOT_2_2     (ROOT_2 / 2.0)

#define ROOT_3       1.73205080757
#define ROOT_3_3     (ROOT_3 / 3.0)

// Compare 
#define EPSILON (f64)0.00001
b32 req(f64 a, f64 b);

// Util
#define CLAMP(x, a, b) (x < a ? a : x > b ? b : x);

// Trig wrappers
#define COS(r) (f64)cos((f64)r)
#define SIN(r) (f64)sin((f64)r)

//------------------------------------------------------------------------------
// modified prof.h from performance aware programming series, haversine project
// See https://www.computerenhance.com/ for more

#define QUICK_BUILD
#ifdef QUICK_BUILD
// x86intrin is huge and slows compilation down massively. We only need 
// __rdtsc from this library, so just replace with a direct call.
static inline u64 __rdtsc(){
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((u64)hi << 32) | lo;
}
#else
#include "x86intrin.h"
#endif

static inline u64 prof_get_os_timer_freq() {
  return 1000000;
}

static inline u64 prof_read_os_timer() {
  struct timeval value;
  gettimeofday(&value, 0);

  return prof_get_os_timer_freq() * (u64)value.tv_sec + (u64)value.tv_usec;
}

static inline u64 prof_read_cpu_timer() {
  return __rdtsc();
}

static inline u64 prof_estimate_cpu_freq(u64 wait_ms) {
  u64 os_freq = prof_get_os_timer_freq();

  u64 cpu_start = prof_read_cpu_timer();
  u64 os_start = prof_read_os_timer();
  u64 os_end = 0;
  u64 os_elapsed = 0;
  u64 os_wait_time = os_freq * wait_ms / 1000;

  while (os_elapsed < os_wait_time) {
    os_end = prof_read_os_timer();
    os_elapsed = os_end - os_start;
  }

  u64 cpu_end = prof_read_cpu_timer();
  u64 cpu_elapsed = cpu_end - cpu_start;
  u64 cpu_freq = 0;

  if (os_elapsed) {
    cpu_freq = os_freq * cpu_elapsed / os_elapsed;
  }

  return cpu_freq;
}

//------------------------------------------------------------------------------
// Defines

#define MAX_INTERSECTIONS 16
#define MAX_OBJECTS 512
#define MAX_LIGHTS 512
#define MAX_DEPTH 5

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define INTERSECTION_APPEND_BOUND_CHECK

#ifdef INTERSECTION_APPEND_BOUND_CHECK
#define BOUND_CHECK(_is)\
  if (_is->count >= MAX_INTERSECTIONS) {\
    fprintf(stderr, "Too many intersections\n");\
    exit(1);\
  }
#else
#define BOUND_CHECK(...)
#endif

#define INTERSECTION_APPEND(_is, _t, _o) \
  BOUND_CHECK(_is)\
  _is->xs[_is->count].t = _t;\
  _is->xs[_is->count++].o = _o

//------------------------------------------------------------------------------
// Types

typedef struct {
  f64 x;
  f64 y;
} v2;

typedef f64 vec4[4];
typedef f64 matrix4[16];

typedef struct {
  u32 hsize;
  u32 vsize;
  f64 fov;
  f64 half_width;
  f64 half_height;
  f64 pixel_size;
  matrix4 transform;
  matrix4 inverse_transform;
} camera;

typedef struct {
  u64 start;
  u64 end;
  u64 width;
  u64 height;
  // NOTE: total rays once shadowing is implemented?
} render_stats;

typedef struct {
  u32 width;
  u32 height;
  vec4 *pixels;
} canvas;

enum light_type { PointLightType };

typedef struct {
  enum light_type type;
  vec4 position;
  vec4 intensity;
  /*
  union {
    struct {} point_light;
  } value;
  */
} light;

enum pattern_type { 
  StripedPatternType, GradientPatternType, RingPatternType, CheckerPatternType, 
};

typedef struct {
  enum pattern_type type;
  matrix4 transform;
  matrix4 inverse_transform;
  union {
    struct {
      vec4 a;
      vec4 b;
    } striped;
    struct {
      vec4 a;
      vec4 b;
    } gradient;
    struct {
      vec4 a;
      vec4 b;
    } ring;
    struct {
      vec4 a;
      vec4 b;
    } checker;
  } value;
} pattern;

#define REFRACT_VACUUM   1
#define REFRACT_AIR      1.00029
#define REFRACT_WATER    1.333
#define REFRACT_GLASS    1.52
#define REFRACT_DIAMOND  2.417

typedef struct {
  vec4 color;
  f64 ambient;
  f64 diffuse;
  f64 specular;
  f64 shininess;
  f64 reflective;
  f64 transparency;
  f64 refractive_index;
  pattern *p;
} material;

enum object_type { SphereType, PlaneType, CubeType };

typedef struct {
  enum object_type type;
  matrix4 transform;
  matrix4 inverse_transform;
  material material;
  /*
  union {
    struct {} sphere;
  } value;
  */
} object;

typedef struct {
  f64 t;
  const object *o;
} intersection;


typedef struct {
  u32 count;
  intersection xs[MAX_INTERSECTIONS];
} intersection_group;


typedef struct {
  vec4 origin;
  vec4 direction;
} ray;

typedef struct {
  f64 t;
  vec4 point;
  vec4 eyev;
  vec4 normalv;
  vec4 reflectv;
  vec4 over_point;
  vec4 under_point;
  b32 inside;
  f64 n1;
  f64 n2;
  const object *o;
} computations;


typedef struct {
  object objects[MAX_OBJECTS];
  u32 objects_count;

  light lights[MAX_LIGHTS];
  u32 lights_count;
} world;

//------------------------------------------------------------------------------
// Functions

void camera_init(camera *c, const u32 hsize, const u32 vsize, const f64 fov);
void camera_set_transform(camera *c, const matrix4 T);

void camera_ray_for_pixel(const camera *c, const u32 x, const u32 y, ray *out);

canvas *camera_render(const camera *v, const world *w, render_stats *s);

void render_stats_print(const render_stats *s);

canvas *canvas_alloc(u32 width, u32 height);
void canvas_free(canvas *c);

void canvas_write(canvas *c, u32 x, u32 y, vec4 color);
void canvas_safe_write(canvas *c, s32 x, s32 y, vec4 color);
vec4 *canvas_at(const canvas *c, u32 x, u32 y);

char *canvas_to_ppm(const canvas *c);

void light_init(light *o, const vec4 position, const vec4 intensity);
void point_light_init(light *o, const vec4 position, const vec4 intensity);

void pattern_init(pattern *p);
void pattern_set_transform(pattern *p, const matrix4 T);
void pattern_color_at(const pattern *p, const vec4 l, vec4 out);
void pattern_object_color_at(const pattern *p, const object *o, const vec4 l, vec4 out);

void striped_pattern_init(pattern *p, const vec4 a, const vec4 b);
void gradient_pattern_init(pattern *p, const vec4 a, const vec4 b);
void ring_pattern_init(pattern *p, const vec4 a, const vec4 b);
void checker_pattern_init(pattern *p, const vec4 a, const vec4 b);

void material_init(material *m);

void material_lighting(const material *m, const light *l, const object *o, const vec4 position, const vec4 eyev, const vec4 normalv, const b32 in_shadow, vec4 result);

void matrix4_print(const matrix4 a);

extern const matrix4 IDENTITY;

// Used for indexing like A[2 _ 3] = A[2][3] = A[2 * 4 + 3] = A[11]
#define _ * 4 +

void matrix4_transpose(const matrix4 A, matrix4 out);
void matrix4_inverse(const matrix4 A, matrix4 out);
f64 matrix4_det(const matrix4 A);

b32 matrix4_eq(const matrix4 A, const matrix4 B);
void matrix4_mul(const matrix4 A, const matrix4 B, matrix4 out);

void matrix4_mulv(const matrix4 A, const vec4 b, vec4 out);

void object_init(object *o);
void object_set_transform(object *o, const matrix4 T);
void object_set_material(object *o, const material *M);
void object_normal_at(const object *o, const vec4 p, vec4 out);

void sphere_init(object *o);
void glass_sphere_init(object *o);
void plane_init(object *o);
void cube_init(object *o);

v2 cube_check_axis(f64 origin, f64 direction);

int intersection_compare(const void* a, const void* b);

const intersection *intersection_group_hit(const intersection_group *ig);

void ray_position(const ray *r, f64 t, vec4 out);
void ray_intersect(const ray *r, const object *o, intersection_group *ig);
void ray_transform(const ray *r, const matrix4 T, ray *out);

void computations_prepare(const intersection *i, const ray *r, const intersection_group *ig, computations *out);
f64 computations_schlick(const computations *comps);

void vec4_print(const vec4 a);

#define point4_init(x, y, z) { x, y, z, 1.0 }
#define point4(x, y, z) (vec4)point4_init(x,y,z)
#define vec4_init(x, y, z) { x, y, z, 0.0 }
#define vec4(x, y, z) (vec4)vec4_init(x,y,z)

extern const vec4 ZERO_VEC;

// NOTE: 4th channel is always unused for colors. Could be alpha channel or
// removed.
#define color_init(r, g, b) { r, g, b, 0.0 }
#define color(r, g, b) (vec4)color_init(r,g,b)

extern const vec4 BLACK;
extern const vec4 WHITE;

b32 is_point4(const vec4 a);
b32 is_vec4(const vec4 a);
f64 vec4_mag(const vec4 a);
void vec4_neg(const vec4 a, vec4 out);
void vec4_norm(const vec4 a, vec4 out);

b32 vec4_eq(const vec4 a, const vec4 b);
void vec4_add(const vec4 a, const vec4 b, vec4 out);
void vec4_sub(const vec4 a, const vec4 b, vec4 out);
void vec4_mul(const vec4 a, const vec4 b, vec4 out);
void vec4_div(const vec4 a, const vec4 b, vec4 out);
void vec4_cross(const vec4 a, const vec4 b, vec4 out);
void vec4_reflect(const vec4 v, const vec4 n, vec4 out);

void vec4_scale(const vec4 a, f64 b, vec4 out);

f64 vec4_dot(const vec4 a, const vec4 b);

void translation(f64 x, f64 y, f64 z, matrix4 out);
void scaling(f64 x, f64 y, f64 z, matrix4 out);

void rotation_x(f64 r, matrix4 out);
void rotation_y(f64 r, matrix4 out);
void rotation_z(f64 r, matrix4 out);

void shearing(f64 xy, f64 xz, f64 yx, f64 yz, f64 zx, f64 zy, matrix4 out);

void view_transform(vec4 from, vec4 to, vec4 up, matrix4 out);

void world_init(world *w);
void world_intersect(const world *w, const ray *r, intersection_group *ig);
void world_shade_hit(const world *w, const computations *c, u64 depth, vec4 out);
void world_reflected_color(const world *w, const computations *c, u64 depth, vec4 out);
void world_color_at(const world *w, const ray *r, u64 depth, vec4 out);
void world_refracted_color(const world *w, const computations *c, u64 depth, vec4 out);

b32 world_is_shadowed(const world *w, const light *l, const vec4 p);

#endif
