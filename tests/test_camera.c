
#include "tests.h"

void test_camera(void)
{
  TESTS();

  TEST {
      // Default camera
      camera c = {0};
      camera_init(&c, 160, 120, PI_2);

      assert(c.hsize == 160);
      assert(c.vsize == 120);
      assert(req(c.fov, PI_2));
      assert(m4_eq(c.transform, IDENTITY));
  }

  TEST {
      // Pixel size for horizontal canvas
      camera c = {0};
      camera_init(&c, 200, 125, PI_2);

      assert(req(c.pixel_size, 0.01));
  }

  TEST {
      // Pixel size for vertical canvas
      camera c = {0};
      camera_init(&c, 125, 200, PI_2);

      assert(req(c.pixel_size, 0.01));
  }

  TEST {
      // Ray through center of canvas
      camera c = {0};
      camera_init(&c, 201, 101, PI_2);

      ray r = {0};

      camera_ray_for_pixel(&c, 100, 50, &r);

      assert(v4_eq(r.origin, point(0, 0, 0)));
      assert(v4_eq(r.direction, vector(0, 0, -1)));
  }

  TEST {
      // Ray through corner of canvas
      camera c = {0};
      camera_init(&c, 201, 101, PI_2);

      ray r = {0};

      camera_ray_for_pixel(&c, 0, 0, &r);

      assert(v4_eq(r.origin, point(0, 0, 0)));
      assert(v4_eq(r.direction, vector(0.66519, 0.33259, -0.66851)));
  }

  TEST {
      // Ray through canvas when camera is transformed
      camera c = {0};
      camera_init(&c, 201, 101, PI_2);

      {
        m4 R = {0};
        rotation_y(PI_4, R);

        m4 T = {0};
        translation(0, -2, 5, T);

        m4 Z = {0};
        m4_mul(R, T, Z);

        camera_set_transform(&c, Z);
      }

      ray r = {0};

      camera_ray_for_pixel(&c, 100, 50, &r);

      assert(v4_eq(r.origin, point(0, 2, -5)));
      assert(v4_eq(r.direction, vector(ROOT_2_2, 0, -ROOT_2_2)));
  }

  TEST {
      world w = {0};
      world_init(&w);

      camera v = {0};
      camera_init(&v, 11, 11, PI_2);

      v4 from = point_init(0, 0, -5);
      v4 to = point_init(0, 0, 0);
      v4 up = vector_init(0, 1, 0);

      m4 T = {0};
      view_transform(from, to, up, T);

      camera_set_transform(&v, T);

      canvas *c = camera_render(&v, &w, NULL);
      assert(c != NULL);

      v3 *px = canvas_at(c, 5, 5);
      assert(px != NULL);

      assert(v3_eq(*px, color(0.38066, 0.47583, 0.2855)));

      canvas_free(c);
  }
}

