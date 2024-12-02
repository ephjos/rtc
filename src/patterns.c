#include "rtc.h"

void pattern_init(pattern *p)
{
  pattern_set_transform(p, IDENTITY);
}

void striped_pattern_init(pattern *p, const v3 a, const v3 b)
{
  pattern_init(p);

  p->type = StripedPatternType;
  memcpy(p->value.striped.a, a, sizeof(v3));
  memcpy(p->value.striped.b, b, sizeof(v3));
}

void gradient_pattern_init(pattern *p, const v3 a, const v3 b)
{
  pattern_init(p);

  p->type = GradientPatternType;
  memcpy(p->value.gradient.a, a, sizeof(v3));
  memcpy(p->value.gradient.b, b, sizeof(v3));
}

void ring_pattern_init(pattern *p, const v3 a, const v3 b)
{
  pattern_init(p);

  p->type = RingPatternType;
  memcpy(p->value.gradient.a, a, sizeof(v3));
  memcpy(p->value.gradient.b, b, sizeof(v3));
}

void checker_pattern_init(pattern *p, const v3 a, const v3 b)
{
  pattern_init(p);

  p->type = CheckerPatternType;
  memcpy(p->value.gradient.a, a, sizeof(v3));
  memcpy(p->value.gradient.b, b, sizeof(v3));
}

void pattern_set_transform(pattern *p, const m4 T)
{
  memcpy(p->transform, T, sizeof(m4));
  m4_inverse(p->transform, p->inverse_transform);
}

void pattern_color_at(const pattern *p, const v4 l, v3 out)
{
  switch (p->type) {
    case StripedPatternType: {
      u64 ix = (u64)floor(l[0]);

      if (ix % 2 == 0) {
        memcpy(out, p->value.striped.a, sizeof(v3));
      } else {
        memcpy(out, p->value.striped.b, sizeof(v3));
      }
    } break;
    case GradientPatternType: {
      v3 distance = {0};
      v3_sub(p->value.gradient.b, p->value.gradient.a, distance);

      f64 fraction = l[0] - floor(l[0]);

      v3_scale(distance, fraction, out);
      v3_add(p->value.gradient.a, out, out);
    } break;
    case RingPatternType: {
      u64 ix = (u64)floor((l[0] * l[0]) + (l[2] * l[2]));

      if (ix % 2 == 0) {
        memcpy(out, p->value.striped.a, sizeof(v3));
      } else {
        memcpy(out, p->value.striped.b, sizeof(v3));
      }
    } break;
    case CheckerPatternType: {
      u64 ix = (u64)(floor(l[0]+EPSILON) + floor(l[1]+EPSILON) + floor(l[2]+EPSILON));

      if (ix % 2 == 0) {
        memcpy(out, p->value.striped.a, sizeof(v3));
      } else {
        memcpy(out, p->value.striped.b, sizeof(v3));
      }
    } break;
  }
}

void pattern_object_color_at(const pattern *p, const object *o, const v4 l, v3 out)
{
  v4 object_point = {0};
  m4_mulv(o->inverse_transform, l, object_point);

  v4 pattern_point = {0};
  m4_mulv(p->inverse_transform, object_point, pattern_point);

  pattern_color_at(p, pattern_point, out);
}

