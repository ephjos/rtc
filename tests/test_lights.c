
#include "tests.h"

void test_lights()
{
  TESTS();

  TEST {
      // A point light has a position and intensity
      vec4 intensity = color_init(1, 1, 1);
      vec4 position = point4_init(0, 0, 0);

      light l = {0};
      point_light_init(&l, position, intensity);

      assert(vec4_eq(l.position, position));
      assert(vec4_eq(l.intensity, intensity));
  }
}

