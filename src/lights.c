#include "rtc.h"

void light_init(light *o, const vec4 position, const vec4 intensity)
{
  memcpy(o->position, position, sizeof(vec4));
  memcpy(o->intensity, intensity, sizeof(vec4));
}

void point_light_init(light *o, const vec4 position, const vec4 intensity)
{
  o->type = PointLightType;
  light_init(o, position, intensity);
}

