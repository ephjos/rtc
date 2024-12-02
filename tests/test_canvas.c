
#include "tests.h"

void test_canvas()
{
  TESTS();

  TEST {
      // Creating a canvas
      canvas *c = canvas_alloc(10, 20);
      assert(c->width == 10);
      assert(c->height == 20);

      for (u32 i = 0; i < c->height; i++) {
        for (u32 j = 0; j < c->width; j++) {
          assert(v3_eq(c->pixels[i * c->width + j], color(0.0, 0.0, 0.0)));
        }
      }

      canvas_free(c);
  }

  TEST {
      // Writing pixels to canvas
      canvas *c = canvas_alloc(10, 20);
      v3 red = color_init(1.0, 0.0, 0.0);

      canvas_write(c, 2, 3, red);
      assert(v3_eq(*canvas_at(c, 2, 3), red));

      canvas_free(c);
  }

  TEST {
      // Constructing PPM string
      canvas *c = canvas_alloc(5, 3);
      v3 c1 = color_init(1.5, 0.0, 0.0);
      v3 c2 = color_init(0.0, 0.5, 0.0);
      v3 c3 = color_init(-0.5, 0.0, 1.0);

      canvas_write(c, 0, 0, c1);
      canvas_write(c, 2, 1, c2);
      canvas_write(c, 4, 2, c3);

      char *ppm = canvas_to_ppm(c);
      char *expected = "P3\n5 3\n255\n255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 127 0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 \n";
      assert(strcmp(ppm, expected) == 0);

      free(ppm);
      canvas_free(c);
  }
}
