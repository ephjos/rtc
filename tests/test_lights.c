
#include "tests.h"

void test_lights()
{
  TESTS();

  TEST {
      // A point light has a position and intensity
      v4 position = point_init(0, 0, 0);
      v3 intensity = color_init(1, 1, 1);

      light l = {0};
      point_light_init(&l, position, intensity);

      assert(v4_eq(l.position, position));
      assert(v3_eq(l.intensity, intensity));
  }
}

