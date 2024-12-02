#include "rtc.h"

void object_init(object *o)
{
  object_set_transform(o, IDENTITY);

  material m = {0};
  material_init(&m);
  object_set_material(o, &m);
}

void object_set_transform(object *o, const m4 T)
{
  memcpy(o->transform, T, sizeof(m4));
  m4_inverse(o->transform, o->inverse_transform);
}

void object_set_material(object *o, const material *m)
{
  o->material = *m;
}

void sphere_init(object *o)
{
  object_init(o);
  o->type = SphereType;
}

void glass_sphere_init(object *o)
{
  object_init(o);
  o->type = SphereType;

  o->material.transparency = 1.0;
  o->material.refractive_index = 1.5;
}

void plane_init(object *o)
{
  object_init(o);
  o->type = PlaneType;
}

void cube_init(object *o)
{
  object_init(o);
  o->type = CubeType;
}

void object_normal_at(const object *o, const v4 p, v4 out)
{
  v4 object_point = {0};
  m4_mulv(o->inverse_transform, p, object_point);

  v4 object_normal = {0};

  switch (o->type) {
    case SphereType: {
      v4_sub(object_point, point(0, 0, 0), object_normal);
    } break;
    case PlaneType: {
      memcpy(object_normal, vector(0, 1, 0), sizeof(v4));
    } break;
    case CubeType: {
      f64 x = object_point[0];
      f64 y = object_point[1];
      f64 z = object_point[2];
      f64 ax = fabs(x);
      f64 ay = fabs(y);
      f64 az = fabs(z);
      f64 maxc = MAX(MAX(ax, ay), az);

      if (maxc == ax) {
        memcpy(object_normal, vector(x, 0, 0), sizeof(v4));
      } else if (maxc == ay) {
        memcpy(object_normal, vector(0, y, 0), sizeof(v4)); 
      } else {
        memcpy(object_normal, vector(0, 0, z), sizeof(v4)); 
      }
    } break;
  }

  m4 world_transform = {0};
  m4_transpose(o->inverse_transform, world_transform);

  m4_mulv(world_transform, object_normal, out);
  out[3] = 0.0;
  v4_norm(out, out);
}

void ray_position(const ray *r, f64 t, v4 out)
{
  v4_scale(r->direction, t, out);
  v4_add(r->origin, out, out);
}

void ray_intersect(const ray *input_r, const object *o, intersection_group *ig)
{
  ray r = {0};
  ray_transform(input_r, o->inverse_transform, &r);

  switch (o->type) {
    case SphereType: {
      v4 sphere_to_ray = {0};
      v4_sub(r.origin, point(0, 0, 0), sphere_to_ray);

      f64 a = v4_dot(r.direction, r.direction);
      f64 b = 2 * v4_dot(r.direction, sphere_to_ray);
      f64 c = v4_dot(sphere_to_ray, sphere_to_ray) - 1;

      f64 discriminant = (b*b) - 4 * a * c;

      if (discriminant >= 0) {
        f64 root_discriminant = (f64)sqrt(discriminant);
        f64 t1 = (-b - root_discriminant) / (2*a);
        f64 t2 = (-b + root_discriminant) / (2*a);

        INTERSECTION_APPEND(ig, t1, o);
        INTERSECTION_APPEND(ig, t2, o);
      }
    } break;
    case PlaneType: {
      if (fabs(r.direction[1]) >= EPSILON) {
        f64 t = -r.origin[1] / r.direction[1];

        INTERSECTION_APPEND(ig, t, o);
      }
    } break;
    case CubeType: {
      v2 xt = {0};
      cube_check_axis(r.origin[0], r.direction[0], xt);
      v2 yt = {0};
      cube_check_axis(r.origin[1], r.direction[1], yt);
      v2 zt = {0};
      cube_check_axis(r.origin[2], r.direction[2], zt);

      f64 tmin = MAX(MAX(xt[0], yt[0]), zt[0]);
      f64 tmax = MIN(MIN(xt[1], yt[1]), zt[1]);

      if (tmin <= tmax) {
        INTERSECTION_APPEND(ig, tmin, o);
        INTERSECTION_APPEND(ig, tmax, o);
      }
    } break;
  }
}

int intersection_compare(const void* a, const void* b) {
  f64 at = ((intersection*)a)->t;
  f64 bt = ((intersection*)b)->t;

  return (at > bt) - (at < bt);
}

const intersection *intersection_group_hit(const intersection_group *ig)
{
  if (ig->count == 0) {
    return NULL;
  }

  f64 min = FLT_MAX;
  const intersection *result = NULL;
  for (u32 j = 0; j < ig->count; j++) {
    f64 t = ig->xs[j].t;
    if (t >= 0 && t < min) {
      min = t;
      result = &ig->xs[j];
    }
  }

  return result;
}

void ray_transform(const ray *r, const m4 T, ray *out)
{
  m4_mulv(T, r->origin, out->origin);
  m4_mulv(T, r->direction, out->direction);
}

void computations_prepare(const intersection *i, const ray *r, const intersection_group *ig, computations *out)
{
  out->t = i->t;
  out->o = i->o;

  ray_position(r, out->t, out->point);
  v4_neg(r->direction, out->eyev);
  object_normal_at(out->o, out->point, out->normalv);

  f64 normal_dot_eye = v4_dot(out->normalv, out->eyev);
  if (normal_dot_eye < 0) {
    out->inside = true;
    v4_neg(out->normalv, out->normalv);
  }

  v4_scale(out->normalv, (f64)EPSILON, out->over_point);
  v4_add(out->point, out->over_point, out->over_point);

  v4_scale(out->normalv, (f64)EPSILON, out->under_point);
  v4_sub(out->point, out->under_point, out->under_point);

  v4_reflect(r->direction, out->normalv, out->reflectv);

  if (ig == NULL) {
    return;
  }

  // Refract
  // NOTE: could this be a stack?
  u32 containers_index = 0;
  u32 containers_length = 0;
  #define CONTAINER_CAPACITY MAX_INTERSECTIONS
  const object *containers[CONTAINER_CAPACITY] = {NULL};

  for (u64 j = 0; j < ig->count; j++) {
    // NOTE: treating i as hit for now, may need to calculate here
    intersection curr = ig->xs[j];
    if (req(i->t, curr.t) && i->o == curr.o) {
      if (containers_length == 0) {
        out->n1 = 1.0;
      } else {
        out->n1 = containers[containers_index-1]->material.refractive_index;
      }
    }

    s32 found_index = -1;
    for (u64 k = 0; k < CONTAINER_CAPACITY; k++) {
      if (containers[k] != NULL && containers[k] == curr.o) {
        found_index = k;
        break;
      }
    }

    if (found_index != -1) {
      containers[found_index] = NULL;
      containers_length--;

      // Update index to point after last defined item
      for (s64 k = CONTAINER_CAPACITY-1; k >= 0; k--) {
        if (containers[k] != NULL) {
          containers_index = k+1;
          break;
        }
      }
    } else {
      containers[containers_index++] = curr.o;
      containers_length++;
    }

    if (req(i->t, curr.t) && i->o == curr.o) {
      if (containers_length == 0) {
        out->n2 = 1.0;
      } else {
        out->n2 = containers[containers_index-1]->material.refractive_index;
      }

      break;
    }
  }
}

f64 computations_schlick(const computations *comps)
{
  f64 cos = v4_dot(comps->eyev, comps->normalv);

  if (comps->n1 > comps->n2) {
    f64 n = comps->n1 / comps->n2;
    f64 sin2_t = (n*n) * (1 - (cos*cos));

    if (sin2_t > 1) {
      return 1;
    }

    f64 cos_t = sqrt(1.0 - sin2_t);
    cos = cos_t;
  }

  f64 r0 = ((comps->n1 - comps->n2) / (comps->n1 + comps->n2));
  r0 *= r0;
  return r0 + (1 - r0) * pow(1 - cos, 5);
}

