
#include "tests.h"

void test_patterns()
{

  TESTS();

  TEST {
    // Creating a stripe pattern
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    assert(vec4_eq(s.value.striped.a, WHITE));
    assert(vec4_eq(s.value.striped.b, BLACK));
  }

  TEST {
    // A stripe pattern is constant in y
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 0, 0), out);
      assert(vec4_eq(out, WHITE));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 1, 0), out);
      assert(vec4_eq(out, WHITE));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 2, 0), out);
      assert(vec4_eq(out, WHITE));
    }
  }

  TEST {
    // A stripe pattern is constant in z
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 0, 0), out);
      assert(vec4_eq(out, WHITE));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 0, 1), out);
      assert(vec4_eq(out, WHITE));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 0, 2), out);
      assert(vec4_eq(out, WHITE));
    }
  }

  TEST {
    // A stripe pattern alternates in x
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0, 0, 0), out);
      assert(vec4_eq(out, WHITE));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(0.9, 0, 0), out);
      assert(vec4_eq(out, WHITE));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(1, 0, 0), out);
      assert(vec4_eq(out, BLACK));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(-0.1, 0, 0), out);
      assert(vec4_eq(out, BLACK));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(-1, 0, 0), out);
      assert(vec4_eq(out, BLACK));
    }

    {
      vec4 out = {0};
      pattern_color_at(&s, point4(-1.1, 0, 0), out);
      assert(vec4_eq(out, WHITE));
    }
  }

  TEST {
    // Stripes with an object transformation
    object s = {0};
    {
      sphere_init(&s);

      matrix4 T = {0};
      scaling(2, 2, 2, T);

      object_set_transform(&s, T);
    }

    pattern p = {0};
    striped_pattern_init(&p, WHITE, BLACK);

    vec4 result = {0};
    pattern_object_color_at(&p, &s, point4(1.5, 0, 0), result);
    assert(vec4_eq(result, WHITE));
  }

  TEST {
    // Stripes with a pattern transformation
    object s = {0};
    sphere_init(&s);

    pattern p = {0};
    striped_pattern_init(&p, WHITE, BLACK);
    {
      matrix4 T = {0};
      scaling(2, 2, 2, T);

      pattern_set_transform(&p, T);
    }

    vec4 result = {0};
    pattern_object_color_at(&p, &s, point4(1.5, 0, 0), result);
    assert(vec4_eq(result, WHITE));
  }

  TEST {
    // Stripes with a pattern and object transformation
    matrix4 T = {0};
    scaling(2, 2, 2, T);

    object s = {0};
    sphere_init(&s);
    object_set_transform(&s, T);

    pattern p = {0};
    striped_pattern_init(&p, WHITE, BLACK);
    pattern_set_transform(&p, T);

    vec4 result = {0};
    pattern_object_color_at(&p, &s, point4(1.5, 0, 0), result);
    assert(vec4_eq(result, WHITE));
  }

  TEST {
    // A gradient linearly interpolates
    pattern p = {0};
    gradient_pattern_init(&p, WHITE, BLACK);

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0.25, 0, 0), result);
      assert(vec4_eq(result, color(0.75, 0.75, 0.75)));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0.5, 0, 0), result);
      assert(vec4_eq(result, color(0.5, 0.5, 0.5)));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0.75, 0, 0), result);
      assert(vec4_eq(result, color(0.25, 0.25, 0.25)));
    }
  }

  TEST {
    // A ring extends in x and z
    pattern p = {0};
    ring_pattern_init(&p, WHITE, BLACK);

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(1, 0, 0), result);
      assert(vec4_eq(result, BLACK));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 1), result);
      assert(vec4_eq(result, BLACK));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0.708, 0, 0.708), result);
      assert(vec4_eq(result, BLACK));
    }
  }

  TEST {
    // A checkerboard repeats in x
    pattern p = {0};
    checker_pattern_init(&p, WHITE, BLACK);

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0.99, 0, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(1.01, 0, 0), result);
      assert(vec4_eq(result, BLACK));
    }
  }

  TEST {
    // A checkerboard repeats in y
    pattern p = {0};
    checker_pattern_init(&p, WHITE, BLACK);

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0.99, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 1.01, 0), result);
      assert(vec4_eq(result, BLACK));
    }
  }

  TEST {
    // A checkerboard repeats in z
    pattern p = {0};
    checker_pattern_init(&p, WHITE, BLACK);

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 0), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 0.99), result);
      assert(vec4_eq(result, WHITE));
    }

    {
      vec4 result = {0};
      pattern_color_at(&p, point4(0, 0, 1.01), result);
      assert(vec4_eq(result, BLACK));
    }
  }

}

