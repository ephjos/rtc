#include "rtc.h"

void pattern_init(pattern *p)
{
  pattern_set_transform(p, IDENTITY);
}

void striped_pattern_init(pattern *p, const vec4 a, const vec4 b)
{
  pattern_init(p);

  p->type = StripedPatternType;
  memcpy(p->value.striped.a, a, sizeof(vec4));
  memcpy(p->value.striped.b, b, sizeof(vec4));
}

void gradient_pattern_init(pattern *p, const vec4 a, const vec4 b)
{
  pattern_init(p);

  p->type = GradientPatternType;
  memcpy(p->value.gradient.a, a, sizeof(vec4));
  memcpy(p->value.gradient.b, b, sizeof(vec4));
}

void ring_pattern_init(pattern *p, const vec4 a, const vec4 b)
{
  pattern_init(p);

  p->type = RingPatternType;
  memcpy(p->value.gradient.a, a, sizeof(vec4));
  memcpy(p->value.gradient.b, b, sizeof(vec4));
}

void checker_pattern_init(pattern *p, const vec4 a, const vec4 b)
{
  pattern_init(p);

  p->type = CheckerPatternType;
  memcpy(p->value.gradient.a, a, sizeof(vec4));
  memcpy(p->value.gradient.b, b, sizeof(vec4));
}

void pattern_set_transform(pattern *p, const matrix4 T)
{
  memcpy(p->transform, T, sizeof(matrix4));
  matrix4_inverse(p->transform, p->inverse_transform);
}

void pattern_color_at(const pattern *p, const vec4 l, vec4 out)
{
  switch (p->type) {
    case StripedPatternType: {
      u64 ix = (u64)floor(l[0]);

      if (ix % 2 == 0) {
        memcpy(out, p->value.striped.a, sizeof(vec4));
      } else {
        memcpy(out, p->value.striped.b, sizeof(vec4));
      }
    } break;
    case GradientPatternType: {
      vec4 distance = {0};
      vec4_sub(p->value.gradient.b, p->value.gradient.a, distance);

      f64 fraction = l[0] - floor(l[0]);

      vec4_scale(distance, fraction, out);
      vec4_add(p->value.gradient.a, out, out);
    } break;
    case RingPatternType: {
      u64 ix = (u64)floor((l[0] * l[0]) + (l[2] * l[2]));

      if (ix % 2 == 0) {
        memcpy(out, p->value.striped.a, sizeof(vec4));
      } else {
        memcpy(out, p->value.striped.b, sizeof(vec4));
      }
    } break;
    case CheckerPatternType: {
      u64 ix = (u64)(floor(l[0]+EPSILON) + floor(l[1]+EPSILON) + floor(l[2]+EPSILON));

      if (ix % 2 == 0) {
        memcpy(out, p->value.striped.a, sizeof(vec4));
      } else {
        memcpy(out, p->value.striped.b, sizeof(vec4));
      }
    } break;
  }
}

void pattern_object_color_at(const pattern *p, const object *o, const vec4 l, vec4 out)
{
  vec4 object_point = {0};
  matrix4_mulv(o->inverse_transform, l, object_point);

  vec4 pattern_point = {0};
  matrix4_mulv(p->inverse_transform, object_point, pattern_point);

  pattern_color_at(p, pattern_point, out);
}

