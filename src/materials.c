#include "rtc.h"

void material_init(material *m)
{
  memcpy(m->color, color(1, 1, 1), sizeof(v3));
  m->ambient = 0.1;
  m->diffuse = 0.9;
  m->specular = 0.9;
  m->shininess = 200.0;
  m->reflective = 0.0;
  m->transparency = 0.0;
  m->refractive_index = 1.0;
}

void material_lighting(const material *m, const light *l, const object *o, const v4 position, const v4 eyev, const v4 normalv, const b32 in_shadow, v3 result)
{
  v3 c = {0};

  if (m->p != NULL) {
    pattern_object_color_at(m->p, o, position, c);
  } else {
    memcpy(c, m->color, sizeof(v3));
  }

  v3 effective_color = {0};
  v3_mul(c, l->intensity, effective_color);

  v4 lightv = {0};
  v4_sub(l->position, position, lightv);
  v4_norm(lightv, lightv);

  v3 diffuse = {0};
  v3 specular = {0};
  v3 ambient = {0};
  v3_scale(effective_color, m->ambient, ambient);

  f64 light_dot_normal = v4_dot(lightv, normalv);

  b32 calculate_diffuse_and_specular = light_dot_normal >= 0 && !in_shadow;

  if (calculate_diffuse_and_specular) {
    v3_scale(effective_color, m->diffuse * light_dot_normal, diffuse);

    v4 neg_lightv = {0};
    v4_neg(lightv, neg_lightv);

    v4 reflectv = {0};
    v4_reflect(neg_lightv, normalv, reflectv);
    f64 reflect_dot_eye = v4_dot(reflectv, eyev);

    if (reflect_dot_eye > 0) {
      f64 factor = (f64)pow(reflect_dot_eye, m->shininess);
      v3_scale(l->intensity, m->specular * factor, specular);
    }
  }

  v3_add(diffuse, result, result);
  v3_add(specular, result, result);
  v3_add(ambient, result, result);
}

