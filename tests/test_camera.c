
#include "tests.h"

void test_camera()
{
  TESTS();

  TEST {
      // Default camera
      camera c = {0};
      camera_init(&c, 160, 120, PI_2);

      assert(c.hsize == 160);
      assert(c.vsize == 120);
      assert(req(c.fov, PI_2));
      assert(matrix4_eq(c.transform, IDENTITY));
  }

  TEST {
      // Pixel size for horizontal canvas
      camera c = {0};
      camera_init(&c, 200, 125, PI_2);

      assert(req(c.pixel_size, 0.01f));
  }

  TEST {
      // Pixel size for vertical canvas
      camera c = {0};
      camera_init(&c, 125, 200, PI_2);

      assert(req(c.pixel_size, 0.01f));
  }

  TEST {
      // Ray through center of canvas
      camera c = {0};
      camera_init(&c, 201, 101, PI_2);

      ray r = {0};

      camera_ray_for_pixel(&c, 100, 50, &r);

      assert(vec4_eq(r.origin, point4(0, 0, 0)));
      assert(vec4_eq(r.direction, vec4(0, 0, -1)));
  }

  TEST {
      // Ray through corner of canvas
      camera c = {0};
      camera_init(&c, 201, 101, PI_2);

      ray r = {0};

      camera_ray_for_pixel(&c, 0, 0, &r);

      assert(vec4_eq(r.origin, point4(0, 0, 0)));
      assert(vec4_eq(r.direction, vec4(0.66519f, 0.33259f, -0.66851f)));
  }

  TEST {
      // Ray through canvas when camera is transformed
      camera c = {0};
      camera_init(&c, 201, 101, PI_2);

      {
        matrix4 R = {0};
        rotation_y(PI_4, R);

        matrix4 T = {0};
        translation(0, -2, 5, T);

        matrix4 Z = {0};
        matrix4_mul(R, T, Z);

        camera_set_transform(&c, Z);
      }

      ray r = {0};

      camera_ray_for_pixel(&c, 100, 50, &r);

      assert(vec4_eq(r.origin, point4(0, 2, -5)));
      assert(vec4_eq(r.direction, vec4(ROOT_2_2, 0, -ROOT_2_2)));
  }

  TEST {
      world w = {0};
      world_init(&w);

      camera v = {0};
      camera_init(&v, 11, 11, PI_2);

      vec4 from = point4_init(0, 0, -5);
      vec4 to = point4_init(0, 0, 0);
      vec4 up = vec4_init(0, 1, 0);

      matrix4 T = {0};
      view_transform(from, to, up, T);

      camera_set_transform(&v, T);

      canvas *c = camera_render(&v, &w, NULL);
      assert(c != NULL);

      vec4 *px = canvas_at(c, 5, 5);
      assert(px != NULL);

      assert(vec4_eq(*px, color(0.38066f, 0.47583f, 0.2855f)));

      canvas_free(c);
  }
}

