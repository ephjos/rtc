#include "rtc.h"

void material_init(material *m)
{
  memcpy(m->color, color(1, 1, 1), sizeof(vec4));
  m->ambient = 0.1;
  m->diffuse = 0.9;
  m->specular = 0.9;
  m->shininess = 200.0;
  m->reflective = 0.0;
  m->transparency = 0.0;
  m->refractive_index = 1.0;
}

void material_lighting(const material *m, const light *l, const object *o, const vec4 position, const vec4 eyev, const vec4 normalv, const b32 in_shadow, vec4 result)
{
  vec4 c = {0};

  if (m->p != NULL) {
    pattern_object_color_at(m->p, o, position, c);
  } else {
    memcpy(c, m->color, sizeof(vec4));
  }

  vec4 effective_color = {0};
  vec4_mul(c, l->intensity, effective_color);

  vec4 lightv = {0};
  vec4_sub(l->position, position, lightv);
  vec4_norm(lightv, lightv);

  vec4 diffuse = {0};
  vec4 specular = {0};
  vec4 ambient = {0};
  vec4_scale(effective_color, m->ambient, ambient);

  f64 light_dot_normal = vec4_dot(lightv, normalv);

  b32 calculate_diffuse_and_specular = light_dot_normal >= 0 && !in_shadow;

  if (calculate_diffuse_and_specular) {
    vec4_scale(effective_color, m->diffuse * light_dot_normal, diffuse);

    vec4 neg_lightv = {0};
    vec4_neg(lightv, neg_lightv);

    vec4 reflectv = {0};
    vec4_reflect(neg_lightv, normalv, reflectv);
    f64 reflect_dot_eye = vec4_dot(reflectv, eyev);

    if (reflect_dot_eye > 0) {
      f64 factor = (f64)pow(reflect_dot_eye, m->shininess);
      vec4_scale(l->intensity, m->specular * factor, specular);
    }
  }

  vec4_add(diffuse, result, result);
  vec4_add(specular, result, result);
  vec4_add(ambient, result, result);
}

