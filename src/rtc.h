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
#include <time.h>


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

#define MAX_INTERSECTIONS 32
#define MAX_OBJECTS 512
#define MAX_LIGHTS 512
#define MAX_DEPTH 5

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define INTERSECTION_INSERT_BOUND_CHECK

#define v4_dot(a, b) ((a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]) + (a[3]*b[3]))

//------------------------------------------------------------------------------
// Types

typedef f64 v2[2];
typedef f64 v3[3];
typedef f64 v4[4];
typedef f64 m4[16];

typedef struct {
  u32 hsize;
  u32 vsize;
  f64 fov;
  f64 half_width;
  f64 half_height;
  f64 pixel_size;
  b32 antialias;
  m4 transform;
  m4 inverse_transform;
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
  v3 *pixels;
} canvas;

enum light_type { PointLightType };

typedef struct {
  enum light_type type;
  v4 position;
  v3 intensity;
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
  m4 transform;
  m4 inverse_transform;
  union {
    struct {
      v3 a;
      v3 b;
    } striped;
    struct {
      v3 a;
      v3 b;
    } gradient;
    struct {
      v3 a;
      v3 b;
    } ring;
    struct {
      v3 a;
      v3 b;
    } checker;
  } value;
} pattern;

#define REFRACT_VACUUM   1
#define REFRACT_AIR      1.00029
#define REFRACT_WATER    1.333
#define REFRACT_GLASS    1.52
#define REFRACT_DIAMOND  2.417

typedef struct {
  v3 color;
  f64 ambient;
  f64 diffuse;
  f64 specular;
  f64 shininess;
  f64 reflective;
  f64 transparency;
  f64 refractive_index;
  pattern *p;
} material;

enum object_type { SphereType, PlaneType, CubeType, CylinderType, ConeType };

typedef struct {
  enum object_type type;
  m4 transform;
  m4 inverse_transform;
  material material;
  union {
    struct {
      f64 minimum;
      f64 maximum;
      b32 closed;
    } cylinder;
    struct {
      f64 minimum;
      f64 maximum;
      b32 closed;
    } cone;
  } value;
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
  v4 origin;
  v4 direction;
} ray;

typedef struct {
  f64 t;
  v4 point;
  v4 eyev;
  v4 normalv;
  v4 reflectv;
  v4 over_point;
  v4 under_point;
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
void camera_set_transform(camera *c, const m4 T);

void camera_ray_for_pixel(const camera *c, const u32 x, const u32 y, ray *out);
void camera_raw_ray_for_pixel(const camera *c, const f64 x_offset, const f64 y_offset, ray *out);

canvas *camera_render(const camera *v, const world *w, render_stats *s);

void render_stats_print(const render_stats *s);

canvas *canvas_alloc(u32 width, u32 height);
void canvas_free(canvas *c);

void canvas_write(canvas *c, u32 x, u32 y, v3 color);
void canvas_safe_write(canvas *c, s32 x, s32 y, v3 color);
v3 *canvas_at(const canvas *c, u32 x, u32 y);

char *canvas_to_ppm(const canvas *c);

void light_init(light *o, const v4 position, const v3 intensity);
void point_light_init(light *o, const v4 position, const v3 intensity);

void pattern_init(pattern *p);
void pattern_set_transform(pattern *p, const m4 T);
void pattern_color_at(const pattern *p, const v4 l, v3 out);
void pattern_object_color_at(const pattern *p, const object *o, const v4 l, v3 out);

void striped_pattern_init(pattern *p, const v3 a, const v3 b);
void gradient_pattern_init(pattern *p, const v3 a, const v3 b);
void ring_pattern_init(pattern *p, const v3 a, const v3 b);
void checker_pattern_init(pattern *p, const v3 a, const v3 b);

void material_init(material *m);

void material_lighting(const material *m, const light *l, const object *o, const v4 position, const v4 eyev, const v4 normalv, const b32 in_shadow, v3 result);

void m4_print(const m4 a);

extern const m4 IDENTITY;

// Used for indexing like A[2 _ 3] = A[2][3] = A[2 * 4 + 3] = A[11]
#define _ * 4 +

void m4_transpose(const m4 A, m4 out);
void m4_inverse(const m4 A, m4 out);
f64 m4_det(const m4 A);

b32 m4_eq(const m4 A, const m4 B);

void object_init(object *o);
void object_set_transform(object *o, const m4 T);
void object_set_material(object *o, const material *M);
void object_normal_at(const object *o, const v4 p, v4 out);

void sphere_init(object *o);
void glass_sphere_init(object *o);
void plane_init(object *o);
void cube_init(object *o);
void cylinder_init(object *o);
void cone_init(object *o);

int intersection_compare(const void* a, const void* b);

const intersection *intersection_group_hit(const intersection_group *ig);

void ray_position(const ray *r, f64 t, v4 out);
void ray_intersect(const ray *r, const object *o, intersection_group *ig);

void computations_prepare(const intersection *i, const ray *r, const intersection_group *ig, computations *out);
f64 computations_schlick(const computations *comps);

extern const v3 BLACK;
extern const v3 WHITE;

void v3_print(const v3 a);

b32 v3_eq(const v3 a, const v3 b);
void v3_mul(const v3 a, const v3 b, v3 out);

#define color_init(r, g, b) { r, g, b }
#define color(r, g, b) (v3)color_init(r,g,b)

void v4_print(const v4 a);

#define point_init(x, y, z) { (x), (y), (z), 1.0 }
#define point(x, y, z) (v4)point_init((x),(y),(z))
#define vector_init(x, y, z) { (x), (y), (z), 0.0 }
#define vector(x, y, z) (v4)vector_init((x),(y),(z))

extern const v4 ZERO_VEC;

b32 is_point(const v4 a);
b32 is_vector(const v4 a);
f64 v4_mag(const v4 a);
void v4_neg(const v4 a, v4 out);
void v4_norm(const v4 a, v4 out);

b32 v4_eq(const v4 a, const v4 b);
void v4_mul(const v4 a, const v4 b, v4 out);
void v4_div(const v4 a, const v4 b, v4 out);
void v4_cross(const v4 a, const v4 b, v4 out);
void v4_reflect(const v4 v, const v4 n, v4 out);

void translation(f64 x, f64 y, f64 z, m4 out);
void scaling(f64 x, f64 y, f64 z, m4 out);

void rotation_x(f64 r, m4 out);
void rotation_y(f64 r, m4 out);
void rotation_z(f64 r, m4 out);

void shearing(f64 xy, f64 xz, f64 yx, f64 yz, f64 zx, f64 zy, m4 out);

void view_transform(v4 from, v4 to, v4 up, m4 out);

void world_init(world *w);
void world_intersect(const world *w, const ray *r, intersection_group *ig);
void world_shade_hit(const world *w, const computations *c, u64 depth, v3 out);
void world_reflected_color(const world *w, const computations *c, u64 depth, v3 out);
void world_color_at(const world *w, const ray *r, u64 depth, v3 out);
void world_refracted_color(const world *w, const computations *c, u64 depth, v3 out);

b32 world_is_shadowed(const world *w, const light *l, const v4 p);

// Static inline functions

static inline void intersection_insert(intersection_group *is, f64 t, const object * o) 
{
#ifdef INTERSECTION_INSERT_BOUND_CHECK
  if (is->count >= MAX_INTERSECTIONS) {\
    fprintf(stderr, "Too many intersections\n");\
      exit(1);\
  }
#endif
  // Insert in order, shifting list as needed
  // NOTE: maybe this could be a linked list for less work on insert? constant
  // time access only matters in test, every operation in practice is a linear
  // scan. However, better cache locality from array may make this
  // implementation faster. Need to test.
  u32 target_index = 0;
  for (; target_index < is->count; target_index++) {
    if (is->xs[target_index].t > t) {
      break;
    }
  }

  for (u32 i = is->count; i > target_index; i--) {
    is->xs[i].t = is->xs[i-1].t;
    is->xs[i].o = is->xs[i-1].o;
  }
  is->count++;

  is->xs[target_index].t = t;
  is->xs[target_index].o = o;
}

static inline void m4_mul(const m4 A, const m4 B, m4 out)
{
  out[0 _ 0] = (A[0 _ 0] * B[0 _ 0]) + (A[0 _ 1] * B[1 _ 0]) + (A[0 _ 2] * B[2 _ 0]) + (A[0 _ 3] * B[3 _ 0]);
  out[0 _ 1] = (A[0 _ 0] * B[0 _ 1]) + (A[0 _ 1] * B[1 _ 1]) + (A[0 _ 2] * B[2 _ 1]) + (A[0 _ 3] * B[3 _ 1]);
  out[0 _ 2] = (A[0 _ 0] * B[0 _ 2]) + (A[0 _ 1] * B[1 _ 2]) + (A[0 _ 2] * B[2 _ 2]) + (A[0 _ 3] * B[3 _ 2]);
  out[0 _ 3] = (A[0 _ 0] * B[0 _ 3]) + (A[0 _ 1] * B[1 _ 3]) + (A[0 _ 2] * B[2 _ 3]) + (A[0 _ 3] * B[3 _ 3]);

  out[1 _ 0] = (A[1 _ 0] * B[0 _ 0]) + (A[1 _ 1] * B[1 _ 0]) + (A[1 _ 2] * B[2 _ 0]) + (A[1 _ 3] * B[3 _ 0]);
  out[1 _ 1] = (A[1 _ 0] * B[0 _ 1]) + (A[1 _ 1] * B[1 _ 1]) + (A[1 _ 2] * B[2 _ 1]) + (A[1 _ 3] * B[3 _ 1]);
  out[1 _ 2] = (A[1 _ 0] * B[0 _ 2]) + (A[1 _ 1] * B[1 _ 2]) + (A[1 _ 2] * B[2 _ 2]) + (A[1 _ 3] * B[3 _ 2]);
  out[1 _ 3] = (A[1 _ 0] * B[0 _ 3]) + (A[1 _ 1] * B[1 _ 3]) + (A[1 _ 2] * B[2 _ 3]) + (A[1 _ 3] * B[3 _ 3]);

  out[2 _ 0] = (A[2 _ 0] * B[0 _ 0]) + (A[2 _ 1] * B[1 _ 0]) + (A[2 _ 2] * B[2 _ 0]) + (A[2 _ 3] * B[3 _ 0]);
  out[2 _ 1] = (A[2 _ 0] * B[0 _ 1]) + (A[2 _ 1] * B[1 _ 1]) + (A[2 _ 2] * B[2 _ 1]) + (A[2 _ 3] * B[3 _ 1]);
  out[2 _ 2] = (A[2 _ 0] * B[0 _ 2]) + (A[2 _ 1] * B[1 _ 2]) + (A[2 _ 2] * B[2 _ 2]) + (A[2 _ 3] * B[3 _ 2]);
  out[2 _ 3] = (A[2 _ 0] * B[0 _ 3]) + (A[2 _ 1] * B[1 _ 3]) + (A[2 _ 2] * B[2 _ 3]) + (A[2 _ 3] * B[3 _ 3]);

  out[3 _ 0] = (A[3 _ 0] * B[0 _ 0]) + (A[3 _ 1] * B[1 _ 0]) + (A[3 _ 2] * B[2 _ 0]) + (A[3 _ 3] * B[3 _ 0]);
  out[3 _ 1] = (A[3 _ 0] * B[0 _ 1]) + (A[3 _ 1] * B[1 _ 1]) + (A[3 _ 2] * B[2 _ 1]) + (A[3 _ 3] * B[3 _ 1]);
  out[3 _ 2] = (A[3 _ 0] * B[0 _ 2]) + (A[3 _ 1] * B[1 _ 2]) + (A[3 _ 2] * B[2 _ 2]) + (A[3 _ 3] * B[3 _ 2]);
  out[3 _ 3] = (A[3 _ 0] * B[0 _ 3]) + (A[3 _ 1] * B[1 _ 3]) + (A[3 _ 2] * B[2 _ 3]) + (A[3 _ 3] * B[3 _ 3]);
}

static inline void m4_mulv(const m4 A, const v4 b, v4 out)
{
  out[0] = (A[0 _ 0] * b[0]) + (A[0 _ 1] * b[1]) + (A[0 _ 2] * b[2]) + (A[0 _ 3] * b[3]);
  out[1] = (A[1 _ 0] * b[0]) + (A[1 _ 1] * b[1]) + (A[1 _ 2] * b[2]) + (A[1 _ 3] * b[3]);
  out[2] = (A[2 _ 0] * b[0]) + (A[2 _ 1] * b[1]) + (A[2 _ 2] * b[2]) + (A[2 _ 3] * b[3]);
  out[3] = (A[3 _ 0] * b[0]) + (A[3 _ 1] * b[1]) + (A[3 _ 2] * b[2]) + (A[3 _ 3] * b[3]);
}


static inline void cube_check_axis(f64 origin, f64 direction, v2 out)
{
  f64 tmin_numerator = (-1 - origin);
  f64 tmax_numerator = (1 - origin);

  f64 tmin = tmin_numerator / direction;
  f64 tmax = tmax_numerator / direction;

  if (tmin > tmax) {
    f64 temp = tmin;
    tmin = tmax;
    tmax = temp;
  }

  out[0] = tmin;
  out[1] = tmax;
}

static inline void v3_add(const v3 a, const v3 b, v3 out)
{
  out[0] = a[0] + b[0];
  out[1] = a[1] + b[1];
  out[2] = a[2] + b[2];
}

static inline void v3_sub(const v3 a, const v3 b, v3 out)
{
  out[0] = a[0] - b[0];
  out[1] = a[1] - b[1];
  out[2] = a[2] - b[2];
}

static inline void v3_scale(const v3 a, f64 b, v3 out)
{
  out[0] = a[0] * b;
  out[1] = a[1] * b;
  out[2] = a[2] * b;
}

static inline void v4_add(const v4 a, const v4 b, v4 out)
{
  out[0] = a[0] + b[0];
  out[1] = a[1] + b[1];
  out[2] = a[2] + b[2];
  out[3] = a[3] + b[3];
}

static inline void v4_sub(const v4 a, const v4 b, v4 out)
{
  out[0] = a[0] - b[0];
  out[1] = a[1] - b[1];
  out[2] = a[2] - b[2];
  out[3] = a[3] - b[3];
}

static inline void v4_scale(const v4 a, f64 b, v4 out)
{
  out[0] = a[0] * b;
  out[1] = a[1] * b;
  out[2] = a[2] * b;
  out[3] = a[3] * b;
}

static inline void ray_transform(const ray *r, const m4 T, ray *out)
{
  m4_mulv(T, r->origin, out->origin);
  m4_mulv(T, r->direction, out->direction);
}

static inline b32 cylinder_check_cap(const ray *r, f64 t)
{
  f64 x = r->origin[0] + t * r->direction[0];
  f64 z = r->origin[2] + t * r->direction[2];

  return (x*x) + (z*z) <= 1;
}

static inline void cylinder_intersect_caps(const ray *r, const object *o, intersection_group *ig)
{
  assert(o->type == CylinderType);
  if (!o->value.cylinder.closed || fabs(r->direction[1]) < EPSILON) {
    return;
  }

  f64 t;

  t = (o->value.cylinder.minimum - r->origin[1]) / r->direction[1];
  if (cylinder_check_cap(r, t)) {
    intersection_insert(ig, t, o);
  }

  t = (o->value.cylinder.maximum - r->origin[1]) / r->direction[1];
  if (cylinder_check_cap(r, t)) {
    intersection_insert(ig, t, o);
  }
}

static inline b32 cone_check_cap(const ray *r, f64 t, f64 radius)
{
  f64 x = r->origin[0] + t * r->direction[0];
  f64 z = r->origin[2] + t * r->direction[2];

  return (x*x) + (z*z) <= fabs(radius);
}

static inline void cone_intersect_caps(const ray *r, const object *o, intersection_group *ig)
{
  assert(o->type == ConeType);
  if (!o->value.cone.closed || fabs(r->direction[1]) < EPSILON) {
    return;
  }

  f64 minimum = o->value.cone.minimum;
  f64 maximum = o->value.cone.maximum;

  f64 t;

  t = (minimum - r->origin[1]) / r->direction[1];
  if (cone_check_cap(r, t, minimum)) {
    intersection_insert(ig, t, o);
  }

  t = (maximum - r->origin[1]) / r->direction[1];
  if (cone_check_cap(r, t, maximum)) {
    intersection_insert(ig, t, o);
  }
}

static inline b32 req(f64 a, f64 b)
{
  return fabs(a - b) < EPSILON;
}

static inline f64 random_uniform()
{
  return (f64)rand()/(f64)RAND_MAX;
}

#endif
