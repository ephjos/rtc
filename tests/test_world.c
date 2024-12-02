
#include "tests.h"

void test_world()
{
  TESTS();

  TEST {
    // Intersect a ray with a world
    world w = {0};
    world_init(&w);

    ray r = {
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {0};
    world_intersect(&w, &r, &ig);

    assert(ig.count == 4);
    assert(req(ig.xs[0].t, 4));
    assert(req(ig.xs[1].t, 4.5));
    assert(req(ig.xs[2].t, 5.5));
    assert(req(ig.xs[3].t, 6));
  }

  TEST {
    // Shade hit
    world w = {0};
    world_init(&w);

    ray r = {
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {0};
    world_intersect(&w, &r, &ig);

    computations c = {0};
    computations_prepare(&ig.xs[0], &r, NULL, &c);

    v3 out = {0};
    world_shade_hit(&w, &c, 0, out);

    assert(v3_eq(out, color(0.38066f, 0.47583f, 0.2855f)));
  }

  TEST {
    // Shade hit from the inside
    world w = {0};
    world_init(&w);

    memcpy(w.lights[0].position, point(0.0, 0.25f, 0.0), sizeof(v4));
    memcpy(w.lights[0].intensity, color(1, 1, 1), sizeof(v3));

    ray r = {
      .origin = point_init(0, 0, 0),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {0};
    world_intersect(&w, &r, &ig);

    computations c = {0};
    computations_prepare(&ig.xs[2], &r, NULL, &c);

    v3 out = {0};
    world_shade_hit(&w, &c, 0, out);

    assert(v3_eq(out, color(0.90498f, 0.90498f, 0.90498f)));
  }

  TEST {
    // The color when a ray misses
    world w = {0};
    world_init(&w);

    ray r = {
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 1, 0),
    };

    v3 out = {0};
    world_color_at(&w, &r, 0, out);

    assert(v3_eq(out, color(0, 0, 0)));
  }

  TEST {
    // The color when a ray hits
    world w = {0};
    world_init(&w);

    ray r = {
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 0, 1),
    };

    v3 out = {0};
    world_color_at(&w, &r, 0, out);

    assert(v3_eq(out, color(0.38066f, 0.47583f, 0.2855f)));
  }

  TEST {
    // The color wiwth an intersection behind the ray
    world w = {0};
    world_init(&w);

    w.objects[0].material.ambient = 1;
    w.objects[1].material.ambient = 1;

    ray r = {
      .origin = point_init(0, 0, 0.75),
      .direction = vector_init(0, 0, -1),
    };

    v3 out = {0};
    world_color_at(&w, &r, 0, out);

    assert(v3_eq(out, w.objects[1].material.color));
  }

  TEST {
    // There is not shadow when nothing is colinear with point and ligh
    world w = {0};
    world_init(&w);

    v4 p = point_init(0, 10, 0);

    b32 is_shadowed = world_is_shadowed(&w, &w.lights[0], p);

    assert(is_shadowed == false);
  }

  TEST {
    // There is shadow when object between point and light
    world w = {0};
    world_init(&w);

    v4 p = point_init(10, -10, 10);

    b32 is_shadowed = world_is_shadowed(&w, &w.lights[0], p);

    assert(is_shadowed == true);
  }

  TEST {
    // There is no shadow when object is behind the light
    world w = {0};
    world_init(&w);

    v4 p = point_init(-20, 20, -20);

    b32 is_shadowed = world_is_shadowed(&w, &w.lights[0], p);

    assert(is_shadowed == false);
  }

  TEST {
    // There is no shadow when object is behind the point
    world w = {0};
    world_init(&w);

    v4 p = point_init(-2, 2, -2);

    b32 is_shadowed = world_is_shadowed(&w, &w.lights[0], p);

    assert(is_shadowed == false);
  }

  TEST {
    object s1 = {0};
    {
      sphere_init(&s1);
    }

    object s2 = {0};
    {
      sphere_init(&s2);

      m4 T = {0};
      translation(0, 0, 10, T);

      object_set_transform(&s2, T);
    }

    light l = {0};
    {
      point_light_init(&l, point(0, 0, -10), color(1, 1, 1));
    }

    // Shade hit is given an intersection in shadow
    world w = {
      .objects_count = 2,
      .objects = {
        s1, s2,
      },
      .lights_count = 1,
      .lights = {
        l
      },
    };

    ray r = {
      .origin = point_init(0, 0, 5),
      .direction = vector_init(0, 0, 1),
    };

    intersection i = {
      .t = 4,
      .o = &s2,
    };

    computations c = {0};
    computations_prepare(&i, &r, NULL, &c);

    v3 out = {0};
    world_shade_hit(&w, &c, 0, out);

    assert(v3_eq(out, color(0.1, 0.1, 0.1)));
  }

  TEST {
    // The reflected color for a nonreflective material
    world w = {0};
    world_init(&w);

    ray r = {
      .origin = point_init(0, 0, 0),
      .direction = vector_init(0, 0, 1),
    };

    w.objects[1].material.ambient = 1;

    intersection i = {
      .t = 1,
      .o = &w.objects[1],
    };

    computations c = {0};
    computations_prepare(&i, &r, NULL, &c);

    v3 out = {0};
    world_reflected_color(&w, &c, 0, out);

    assert(v3_eq(out, color(0, 0, 0)));
  }

  TEST {
    // The reflected color for a reflective material
    world w = {0};
    world_init(&w);

    object p = {0};
    {
      plane_init(&p);

      p.material.reflective = 0.5;

      m4 T = {0};
      translation(0, -1, 0, T);

      object_set_transform(&p, T);
    }

    w.objects[w.objects_count++] = p;

    ray r = {
      .origin = point_init(0, 0, -3),
      .direction = vector_init(0, -ROOT_2_2, ROOT_2_2),
    };

    intersection i = {
      .t = sqrt(2),
      .o = &p,
    };

    computations c = {0};
    computations_prepare(&i, &r, NULL, &c);

    v3 out = {0};
    world_reflected_color(&w, &c, MAX_DEPTH, out);

    assert(v3_eq(out, color(0.19033, 0.23792, 0.14275)));
  }

  TEST {
    // The reflected color for a reflective material from shade hit
    world w = {0};
    world_init(&w);

    object p = {0};
    {
      plane_init(&p);

      p.material.reflective = 0.5;

      m4 T = {0};
      translation(0, -1, 0, T);

      object_set_transform(&p, T);
    }

    w.objects[w.objects_count++] = p;

    ray r = {
      .origin = point_init(0, 0, -3),
      .direction = vector_init(0, -ROOT_2_2, ROOT_2_2),
    };

    intersection i = {
      .t = sqrt(2),
      .o = &p,
    };

    computations c = {0};
    computations_prepare(&i, &r, NULL, &c);

    v3 out = {0};
    world_shade_hit(&w, &c, MAX_DEPTH, out);

    assert(v3_eq(out, color(0.87676, 0.92434, 0.82917)));
  }

  TEST {
    // The reflected color at the maximum recursive depth
    world w = {0};
    world_init(&w);

    object p = {0};
    {
      plane_init(&p);

      p.material.reflective = 0.5;

      m4 T = {0};
      translation(0, -1, 0, T);

      object_set_transform(&p, T);
    }

    w.objects[w.objects_count++] = p;

    ray r = {
      .origin = point_init(0, 0, -3),
      .direction = vector_init(0, -ROOT_2_2, ROOT_2_2),
    };

    intersection i = {
      .t = sqrt(2),
      .o = &p,
    };

    computations c = {0};
    computations_prepare(&i, &r, NULL, &c);

    v3 out = {0};
    world_reflected_color(&w, &c, 0, out);

    assert(v3_eq(out, color(0, 0, 0)));
  }

  TEST {
    // The refracted color with an opaque surface
    world w = {0};
    world_init(&w);

    object *shape = &w.objects[0];

    ray r = {
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {
      .count = 2,
      .xs = {
        { .t = 4, .o = shape, },
        { .t = 6, .o = shape, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[0], &r, &ig, &c);

    v3 result = {0};
    world_refracted_color(&w, &c, 5, result);

    assert(v3_eq(result, color(0, 0, 0)));
  }

  TEST {
    // The refracted color at max depth
    world w = {0};
    world_init(&w);

    object *shape = &w.objects[0];
    shape->material.transparency = 1.0;
    shape->material.refractive_index = 1.5;

    ray r = {
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {
      .count = 2,
      .xs = {
        { .t = 4, .o = shape, },
        { .t = 6, .o = shape, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[0], &r, &ig, &c);

    v3 result = {0};
    world_refracted_color(&w, &c, 0, result);

    assert(v3_eq(result, color(0, 0, 0)));
  }

  TEST {
    // The refracted color under total internal reflection
    world w = {0};
    world_init(&w);

    object *shape = &w.objects[0];
    shape->material.transparency = 1.0;
    shape->material.refractive_index = 1.5;

    ray r = {
      .origin = point_init(0, 0, ROOT_2_2),
      .direction = vector_init(0, 1, 0),
    };

    intersection_group ig = {
      .count = 2,
      .xs = {
        { .t = -ROOT_2_2, .o = shape, },
        { .t = ROOT_2_2, .o = shape, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[1], &r, &ig, &c);

    v3 result = {0};
    world_refracted_color(&w, &c, 5, result);

    assert(v3_eq(result, color(0, 0, 0)));
  }

  TEST {
    // The refracted color under total internal reflection
    world w = {0};
    world_init(&w);

    object *A = &w.objects[0];
    A->material.ambient = 1.0;

    object *B = &w.objects[1];
    B->material.transparency = 1.0;
    B->material.refractive_index = 1.5;

    ray r = {
      .origin = point_init(0, 0, 0.1),
      .direction = vector_init(0, 1, 0),
    };

    intersection_group ig = {
      .count = 4,
      .xs = {
        { .t = -0.9899, .o = A, },
        { .t = -0.4899, .o = B, },
        { .t = 0.4899, .o = B, },
        { .t = 0.9899, .o = A, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[2], &r, &ig, &c);

    v4 result = {0};
    world_refracted_color(&w, &c, 5, result);

    // Verified the below manually, not using test_pattern
    // assert(v3_eq(result, color(0, 0.99888, 0.04725)));
    // Here's the calculated color when this is correct
    assert(v3_eq(result, color(0.8, 1.0, 0.6)));
  }

  TEST {
    // Shade hit with a transparent material
    world w = {0};
    world_init(&w);

    object floor = {0};
    {
      plane_init(&floor);

      m4 T = {0};
      translation(0, -1, 0, T);
      object_set_transform(&floor, T);

      floor.material.transparency = 0.5;
      floor.material.refractive_index = 1.5;
    }

    object ball = {0};
    {
      sphere_init(&ball);

      m4 T = {0};
      translation(0, -3.5, -0.5, T);
      object_set_transform(&ball, T);

      memcpy(ball.material.color, color(1, 0, 0), sizeof(v3));
      ball.material.ambient = 0.5;
    }

    w.objects[w.objects_count++] = floor;
    w.objects[w.objects_count++] = ball;

    ray r = {
      .origin = point_init(0, 0, -3),
      .direction = vector_init(0, -ROOT_2_2, ROOT_2_2),
    };

    intersection_group ig = {
      .count = 1,
      .xs = {
        { .t = sqrt(2), .o = &floor },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[0], &r, &ig, &c);

    v3 result = {0};
    world_shade_hit(&w, &c, 5, result);

    assert(v3_eq(result, color(0.93642, 0.68642, 0.68642)));
  }

  TEST {
    // Shade hit with a reflective transparent material
    world w = {0};
    world_init(&w);

    object floor = {0};
    {
      plane_init(&floor);

      m4 T = {0};
      translation(0, -1, 0, T);
      object_set_transform(&floor, T);

      floor.material.reflective = 0.5;
      floor.material.transparency = 0.5;
      floor.material.refractive_index = 1.5;
    }

    object ball = {0};
    {
      sphere_init(&ball);

      m4 T = {0};
      translation(0, -3.5, -0.5, T);
      object_set_transform(&ball, T);

      memcpy(ball.material.color, color(1, 0, 0), sizeof(v3));
      ball.material.ambient = 0.5;
    }

    w.objects[w.objects_count++] = floor;
    w.objects[w.objects_count++] = ball;

    ray r = {
      .origin = point_init(0, 0, -3),
      .direction = vector_init(0, -ROOT_2_2, ROOT_2_2),
    };

    intersection_group ig = {
      .count = 1,
      .xs = {
        { .t = sqrt(2), .o = &floor },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[0], &r, &ig, &c);

    v3 result = {0};
    world_shade_hit(&w, &c, 5, result);

    assert(v3_eq(result, color(0.93391, 0.69643, 0.69243)));
  }
}

