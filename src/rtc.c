#include "rtc.h"

const v3 BLACK = color_init(0, 0, 0);
const v3 WHITE = color_init(1, 1, 1);

b32 req(f64 a, f64 b)
{
  return fabs(a - b) < EPSILON;
}
