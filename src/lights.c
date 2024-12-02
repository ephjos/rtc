#include "rtc.h"

void light_init(light *o, const v4 position, const v3 intensity)
{
  memcpy(o->position, position, sizeof(v4));
  memcpy(o->intensity, intensity, sizeof(v3));
}

void point_light_init(light *o, const v4 position, const v3 intensity)
{
  o->type = PointLightType;
  light_init(o, position, intensity);
}

