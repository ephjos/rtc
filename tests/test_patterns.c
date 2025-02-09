
#include "tests.h"

void test_patterns(void)
{

  TESTS();

  TEST {
    // Creating a stripe pattern
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    assert(v3_eq(s.value.striped.a, WHITE));
    assert(v3_eq(s.value.striped.b, BLACK));
  }

  TEST {
    // A stripe pattern is constant in y
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 0, 0), out);
      assert(v3_eq(out, WHITE));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 1, 0), out);
      assert(v3_eq(out, WHITE));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 2, 0), out);
      assert(v3_eq(out, WHITE));
    }
  }

  TEST {
    // A stripe pattern is constant in z
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 0, 0), out);
      assert(v3_eq(out, WHITE));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 0, 1), out);
      assert(v3_eq(out, WHITE));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 0, 2), out);
      assert(v3_eq(out, WHITE));
    }
  }

  TEST {
    // A stripe pattern alternates in x
    pattern s = {0};
    striped_pattern_init(&s, WHITE, BLACK);

    {
      v3 out = {0};
      pattern_color_at(&s, point(0, 0, 0), out);
      assert(v3_eq(out, WHITE));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(0.9, 0, 0), out);
      assert(v3_eq(out, WHITE));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(1, 0, 0), out);
      assert(v3_eq(out, BLACK));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(-0.1, 0, 0), out);
      assert(v3_eq(out, BLACK));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(-1, 0, 0), out);
      assert(v3_eq(out, BLACK));
    }

    {
      v3 out = {0};
      pattern_color_at(&s, point(-1.1, 0, 0), out);
      assert(v3_eq(out, WHITE));
    }
  }

  TEST {
    // Stripes with an object transformation
    object s = {0};
    {
      sphere_init(&s);

      m4 T = {0};
      scaling(2, 2, 2, T);

      object_set_transform(&s, T);
    }

    pattern p = {0};
    striped_pattern_init(&p, WHITE, BLACK);

    v3 result = {0};
    pattern_object_color_at(&p, &s, point(1.5, 0, 0), result);
    assert(v3_eq(result, WHITE));
  }

  TEST {
    // Stripes with a pattern transformation
    object s = {0};
    sphere_init(&s);

    pattern p = {0};
    striped_pattern_init(&p, WHITE, BLACK);
    {
      m4 T = {0};
      scaling(2, 2, 2, T);

      pattern_set_transform(&p, T);
    }

    v3 result = {0};
    pattern_object_color_at(&p, &s, point(1.5, 0, 0), result);
    assert(v3_eq(result, WHITE));
  }

  TEST {
    // Stripes with a pattern and object transformation
    m4 T = {0};
    scaling(2, 2, 2, T);

    object s = {0};
    sphere_init(&s);
    object_set_transform(&s, T);

    pattern p = {0};
    striped_pattern_init(&p, WHITE, BLACK);
    pattern_set_transform(&p, T);

    v3 result = {0};
    pattern_object_color_at(&p, &s, point(1.5, 0, 0), result);
    assert(v3_eq(result, WHITE));
  }

  TEST {
    // A gradient linearly interpolates
    pattern p = {0};
    gradient_pattern_init(&p, WHITE, BLACK);

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0.25, 0, 0), result);
      assert(v3_eq(result, color(0.75, 0.75, 0.75)));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0.5, 0, 0), result);
      assert(v3_eq(result, color(0.5, 0.5, 0.5)));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0.75, 0, 0), result);
      assert(v3_eq(result, color(0.25, 0.25, 0.25)));
    }
  }

  TEST {
    // A ring extends in x and z
    pattern p = {0};
    ring_pattern_init(&p, WHITE, BLACK);

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(1, 0, 0), result);
      assert(v3_eq(result, BLACK));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 1), result);
      assert(v3_eq(result, BLACK));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0.708, 0, 0.708), result);
      assert(v3_eq(result, BLACK));
    }
  }

  TEST {
    // A checkerboard repeats in x
    pattern p = {0};
    checker_pattern_init(&p, WHITE, BLACK);

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0.99, 0, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(1.01, 0, 0), result);
      assert(v3_eq(result, BLACK));
    }
  }

  TEST {
    // A checkerboard repeats in y
    pattern p = {0};
    checker_pattern_init(&p, WHITE, BLACK);

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0.99, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 1.01, 0), result);
      assert(v3_eq(result, BLACK));
    }
  }

  TEST {
    // A checkerboard repeats in z
    pattern p = {0};
    checker_pattern_init(&p, WHITE, BLACK);

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 0), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 0.99), result);
      assert(v3_eq(result, WHITE));
    }

    {
      v3 result = {0};
      pattern_color_at(&p, point(0, 0, 1.01), result);
      assert(v3_eq(result, BLACK));
    }
  }

}

