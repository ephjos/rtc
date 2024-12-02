
#include "tests.h"

void test_materials()
{
  object sphere = {0};
  sphere_init(&sphere);

  TESTS();

  TEST {
    // The default material
    material m = {0};
    material_init(&m);

    assert(v3_eq(m.color, color(1, 1, 1)));
    assert(req(m.ambient, 0.1));
    assert(req(m.diffuse, 0.9));
    assert(req(m.specular, 0.9));
    assert(req(m.shininess, 200.0));
    assert(req(m.reflective, 0.0));
    assert(req(m.transparency, 0.0));
    assert(req(m.refractive_index, 1.0));
  }

  TEST {
    // Lighting with the eye between the light and the surface
    material m = {0};
    material_init(&m);

    v4 position = point_init(0, 0, 0);

    v4 eyev = vector_init(0, 0, -1);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 0, -10), color(1, 1, 1));

    v3 result = {0};
    material_lighting(&m, &l, &sphere, position, eyev, normalv, false, result);

    assert(v3_eq(result, color(1.9, 1.9, 1.9)));
  }

  TEST {
    // Lighting with the eye between the light and the surface, eye offset 45 deg
    material m = {0};
    material_init(&m);

    v4 position = point_init(0, 0, 0);

    v4 eyev = vector_init(0, ROOT_2_2, -ROOT_2_2);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 0, -10), color(1, 1, 1));

    v3 result = {0};
    material_lighting(&m, &l, &sphere, position, eyev, normalv, false, result);

    assert(v3_eq(result, color(1.0, 1.0, 1.0)));
  }

  TEST {
    // Lighting with the eye opposite surface, light offset 45 deg
    material m = {0};
    material_init(&m);

    v4 position = point_init(0, 0, 0);

    v4 eyev = vector_init(0, 0, -1);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 10, -10), color(1, 1, 1));

    v3 result = {0};
    material_lighting(&m, &l, &sphere, position, eyev, normalv, false, result);

    assert(v3_eq(result, color(0.7364f, 0.7364f, 0.7364f)));
  }

  TEST {
    // Lighting with the eye in the path of the reflection vector
    material m = {0};
    material_init(&m);

    v4 position = point_init(0, 0, 0);

    v4 eyev = vector_init(0, -ROOT_2_2, -ROOT_2_2);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 10, -10), color(1, 1, 1));

    v3 result = {0};
    material_lighting(&m, &l, &sphere, position, eyev, normalv, false, result);

    assert(v3_eq(result, color(1.6364f, 1.6364f, 1.6364f)));
  }

  TEST {
    // Lighting with the light behind the surface
    material m = {0};
    material_init(&m);

    v4 position = point_init(0, 0, 0);

    v4 eyev = vector_init(0, 0, -1);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 0, 10), color(1, 1, 1));

    v3 result = {0};
    material_lighting(&m, &l, &sphere, position, eyev, normalv, false, result);

    assert(v3_eq(result, color(0.1, 0.1, 0.1)));
  }

  TEST {
    // Lighting with the surface in shadow
    material m = {0};
    material_init(&m);

    v4 position = point_init(0, 0, 0);

    v4 eyev = vector_init(0, 0, -1);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 0, -10), color(1, 1, 1));

    v3 result = {0};
    material_lighting(&m, &l, &sphere, position, eyev, normalv, true, result);

    assert(v3_eq(result, color(0.1, 0.1, 0.1)));
  }

  TEST {
    // Lighting with a pattern applied
    pattern s = {0};
    striped_pattern_init(&s, color(1, 1, 1), color(0, 0, 0));

    material m = {0};
    material_init(&m);

    m.p = &s;
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;

    v4 eyev = vector_init(0, 0, -1);
    v4 normalv = vector_init(0, 0, -1);

    light l = {0};
    point_light_init(&l, point(0, 0, -10), color(1, 1, 1));

    v3 c1 = {0};
    material_lighting(&m, &l, &sphere, point(0.9, 0, 0), eyev, normalv, false, c1);
    assert(v3_eq(c1, color(1, 1, 1)));

    v3 c2 = {0};
    material_lighting(&m, &l, &sphere, point(1.1, 0, 0), eyev, normalv, false, c2);
    assert(v3_eq(c2, color(0, 0, 0)));
  }
}

