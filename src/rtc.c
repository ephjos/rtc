#include "rtc.h"

const vec4 BLACK = color_init(0, 0, 0);
const vec4 WHITE = color_init(1, 1, 1);

b32 req(f64 a, f64 b)
{
  return fabs(a - b) < EPSILON;
}
