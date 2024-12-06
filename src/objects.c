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

void cylinder_init(object *o)
{
  object_init(o);
  o->type = CylinderType;
  o->value.cylinder.minimum = -INFINITY;
  o->value.cylinder.maximum = INFINITY;
  o->value.cylinder.closed = false;
}

void cone_init(object *o)
{
  object_init(o);
  o->type = ConeType;
  o->value.cone.minimum = -INFINITY;
  o->value.cone.maximum = INFINITY;
  o->value.cone.closed = false;
}

void object_normal_at(const object *o, const v4 p, v4 out)
{
  v4 object_point = {0};
  m4_mulv(o->inverse_transform, p, object_point);

  v4 object_normal = {0};

  f64 x = object_point[0];
  f64 y = object_point[1];
  f64 z = object_point[2];

  switch (o->type) {
    case SphereType: {
      v4_sub(object_point, point(0, 0, 0), object_normal);
    } break;
    case PlaneType: {
      memcpy(object_normal, vector(0, 1, 0), sizeof(v4));
    } break;
    case CubeType: {
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
    case CylinderType: {
      f64 dist = (x*x) + (z*z);

      if (dist < 1 && y >= o->value.cylinder.maximum - EPSILON) {
        memcpy(object_normal, vector(0, 1, 0), sizeof(v4));
      } else if (dist < 1 && y <= o->value.cylinder.minimum + EPSILON) {
        memcpy(object_normal, vector(0, -1, 0), sizeof(v4));
      } else {
        memcpy(object_normal, vector(x, 0, z), sizeof(v4));
      }
    } break;
    case ConeType: {
      f64 dist = (x*x) + (z*z);

      if (dist < 1 && y >= o->value.cone.maximum - EPSILON) {
        memcpy(object_normal, vector(0, 1, 0), sizeof(v4));
      } else if (dist < 1 && y <= o->value.cone.minimum + EPSILON) {
        memcpy(object_normal, vector(0, -1, 0), sizeof(v4));
      } else {
        f64 ty = sqrt(dist);
        if (y > 0) {
          ty = -ty;
        }
        memcpy(object_normal, vector(x, ty, z), sizeof(v4));
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

  f64 ox = r.origin[0]; f64 oy = r.origin[1]; f64 oz = r.origin[2];
  f64 dx = r.direction[0]; f64 dy = r.direction[1]; f64 dz = r.direction[2];

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
        f64 t0 = (-b - root_discriminant) / (2*a);
        f64 t1 = (-b + root_discriminant) / (2*a);

        intersection_insert(ig, t0, o);
        intersection_insert(ig, t1, o);
      }
    } break;
    case PlaneType: {
      if (fabs(dy) >= EPSILON) {
        f64 t = -oy / dy;

        intersection_insert(ig, t, o);
      }
    } break;
    case CubeType: {
      v2 xt = {0};
      cube_check_axis(ox, dx, xt);
      v2 yt = {0};
      cube_check_axis(oy, dy, yt);
      v2 zt = {0};
      cube_check_axis(oz, dz, zt);

      f64 tmin = MAX(MAX(xt[0], yt[0]), zt[0]);
      f64 tmax = MIN(MIN(xt[1], yt[1]), zt[1]);

      if (tmin <= tmax) {
        intersection_insert(ig, tmin, o);
        intersection_insert(ig, tmax, o);
      }
    } break;
    case CylinderType: {
      f64 a = dx*dx + dz*dz;

      if (fabs(a) > EPSILON) {
        f64 b = 2 * ox * dx + 2 * oz * dz;
        f64 c = ox * ox + oz * oz - 1;

        f64 discriminant = (b*b) - 4 * a * c;

        if (discriminant >= 0) {
          f64 root_discriminant = (f64)sqrt(discriminant);
          f64 t0 = (-b - root_discriminant) / (2*a);
          f64 t1 = (-b + root_discriminant) / (2*a);

          if (t0 > t1) {
            f64 temp = t0;
            t0 = t1;
            t1 = temp;
          }

          f64 minimum = o->value.cylinder.minimum;
          f64 maximum = o->value.cylinder.maximum;

          f64 y0 = oy + t0 * dy;
          if (minimum < y0 && y0 < maximum) {
            intersection_insert(ig, t0, o);
          }

          f64 y1 = oy + t1 * dy;
          if (minimum < y1 && y1 < maximum) {
            intersection_insert(ig, t1, o);
          }
        }
      }

      cylinder_intersect_caps(&r, o, ig);
    } break;
    case ConeType: {
      f64 a = dx*dx - dy*dy + dz*dz;
      f64 b = 2*ox*dx - 2*oy*dy + 2*oz*dz;
      f64 c = ox*ox - oy*oy + oz*oz;

      f64 minimum = o->value.cone.minimum;
      f64 maximum = o->value.cone.maximum;

      if (fabs(a) > EPSILON) {
        f64 discriminant = (b*b) - 4 * a * c;

        if (discriminant >= 0) {
          f64 root_discriminant = (f64)sqrt(discriminant);
          f64 t0 = (-b - root_discriminant) / (2*a);
          f64 t1 = (-b + root_discriminant) / (2*a);

          if (t0 > t1) {
            f64 temp = t0;
            t0 = t1;
            t1 = temp;
          }

          f64 y0 = oy + t0 * dy;
          if (minimum < y0 && y0 < maximum) {
            intersection_insert(ig, t0, o);
          }

          f64 y1 = oy + t1 * dy;
          if (minimum < y1 && y1 < maximum) {
            intersection_insert(ig, t1, o);
          }
        }
      } else if (fabs(b) > EPSILON) {
        f64 t = -c/(2*b);

        f64 y = oy + t * dy;
        if (minimum < y && y < maximum) {
          intersection_insert(ig, t, o);
        }
      }

      cone_intersect_caps(&r, o, ig);
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
    for (s64 k = containers_index; k >= 0; k--) {
      if (containers[k] != NULL && containers[k] == curr.o) {
        found_index = k;
        break;
      }
    }

    if (found_index != -1) {
      containers[found_index] = NULL;
      containers_length--;

      // Update index to point after last defined item
      for (s64 k = containers_index; k >= 0; k--) {
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

