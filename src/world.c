#include "rtc.h"

void world_init(world *w)
{
  // Objects
  w->objects_count = 2;

  {
    object s1 = {0};
    sphere_init(&s1);

    memcpy(s1.material.color, color(0.8, 1.0, 0.6), sizeof(vec4));
    s1.material.diffuse = 0.7;
    s1.material.specular = 0.2;

    w->objects[0] = s1;
  }

  {
    object s2 = {0};
    sphere_init(&s2);

    matrix4 T = {0};
    scaling(0.5, 0.5, 0.5, T);
    object_set_transform(&s2, T);

    w->objects[1] = s2;
  }

  // Lights
  w->lights_count = 1;
  point_light_init(&w->lights[0], point4(-10, 10, -10), color(1, 1, 1));
}

void world_intersect(const world *w, const ray *r, intersection_group *ig)
{
  for (u32 i = 0; i < w->objects_count; i++) {
    ray_intersect(r, &w->objects[i], ig);
  }

  // NOTE: probably want to keep these sorted as we go or find a better sort
  // for the typical intersection size (small)
  qsort(ig->xs, ig->count, sizeof(intersection), intersection_compare);
}

void world_shade_hit(const world *w, const computations *c, u64 depth, vec4 out)
{
  vec4 result = {0};

  for (u32 i = 0; i < w->lights_count; i++) {
    b32 is_shadowed = world_is_shadowed(w, &w->lights[i], c->over_point);

    vec4 surface = {0};
    material_lighting(&c->o->material, &w->lights[i], c->o, c->point, c->eyev, c->normalv, is_shadowed, surface);
    vec4_add(result, surface, result);
  }

  vec4 reflected = {0};
  world_reflected_color(w, c, depth, reflected);

  vec4 refracted = {0};
  world_refracted_color(w, c, depth, refracted);

  if (c->o->material.reflective > 0 && c->o->material.transparency > 0) {
    f64 reflectance = computations_schlick(c);

    vec4_scale(reflected, reflectance, reflected);
    vec4_scale(refracted, (1 - reflectance), refracted);
  }

  vec4_add(result, reflected, result);
  vec4_add(result, refracted, result);

  memcpy(out, result, sizeof(vec4));
}

void world_reflected_color(const world *w, const computations *c, u64 depth, vec4 out)
{
  if (depth == 0 || req(c->o->material.reflective, 0.0)) {
    memcpy(out, color(0, 0, 0), sizeof(vec4));
    return;
  }

  ray reflect_ray = {0};
  memcpy(reflect_ray.origin, c->over_point, sizeof(vec4));
  memcpy(reflect_ray.direction, c->reflectv, sizeof(vec4));

  vec4 result = {0};
  world_color_at(w, &reflect_ray, depth - 1, result);

  vec4_scale(result, c->o->material.reflective, out);
}

void world_color_at(const world *w, const ray *r, u64 depth, vec4 out)
{
  intersection_group ig = {0};
  world_intersect(w, r, &ig);

  const intersection *hit = intersection_group_hit(&ig);
  if (hit == NULL) {
    memset(out, 0, sizeof(vec4));
  } else {
    computations c = {0};
    computations_prepare(hit, r, &ig, &c);

    world_shade_hit(w, &c, depth, out);
  }
}

void world_refracted_color(const world *w, const computations *c, u64 depth, vec4 out)
{

  f64 n_ratio = c->n1 / c->n2;
  f64 cos_i = vec4_dot(c->eyev, c->normalv);
  f64 sin2_t = (n_ratio*n_ratio) * (1 - (cos_i*cos_i));

  if (depth == 0 || sin2_t > 1 || req(c->o->material.transparency, 0)) {
    memcpy(out, color(0, 0, 0), sizeof(vec4));
    return;
  }

  f64 cos_t = sqrt(1.0 - sin2_t);
  vec4 direction = {0};
  {
    vec4 a = {0};
    vec4_scale(c->normalv, n_ratio * cos_i - cos_t, a);

    vec4 b = {0};
    vec4_scale(c->eyev, n_ratio, b);

    vec4_sub(a, b, direction);
  }

  ray refract_ray = {0};
  memcpy(refract_ray.origin, c->under_point, sizeof(vec4));
  memcpy(refract_ray.direction, direction, sizeof(vec4));

  vec4 result = {0};
  world_color_at(w, &refract_ray, depth - 1, result);

  vec4_scale(result, c->o->material.transparency, out);
}

b32 world_is_shadowed(const world *w, const light *l, const vec4 p)
{
  vec4 v = {0};
  vec4_sub(l->position, p, v);

  f64 distance = vec4_mag(v);

  vec4 direction = {0};
  vec4_norm(v, direction);

  ray r = {0};
  memcpy(r.origin, p, sizeof(vec4));
  memcpy(r.direction, direction, sizeof(vec4));

  intersection_group ig = {0};
  world_intersect(w, &r, &ig);

  const intersection *h = intersection_group_hit(&ig);

  b32 is_object_between = h != NULL && h->t < distance;
  return is_object_between;
}

