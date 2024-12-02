
#include "tests.h"

void test_objects()
{
  TESTS();

  TEST {
      // Creating a ray
      ray r = {
        .origin = point_init(1, 2, 3),
        .direction = vector_init(4, 5, 6),
      };
      assert(v4_eq(r.origin, point(1, 2, 3)));
      assert(v4_eq(r.direction, vector(4, 5, 6)));
  }

  TEST {
      ray r = {
        .origin = point_init(2, 3, 4),
        .direction = vector_init(1, 0, 0),
      };

      v4 out = {0};
      ray_position(&r, 0, out);
      assert(v4_eq(out, point(2, 3, 4)));

      ray_position(&r, 1, out);
      assert(v4_eq(out, point(3, 3, 4)));

      ray_position(&r, -1, out);
      assert(v4_eq(out, point(1, 3, 4)));

      ray_position(&r, 2.5, out);
      assert(v4_eq(out, point(4.5, 3, 4)));
  }

  TEST {
      // A ray intersects a sphere
      ray r = {
        .origin = point_init(0, 0, -5),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, 4.0));
      assert(req(ig.xs[1].t, 6.0));
  }

  TEST {
      // A ray intersects a sphere (tangent)
      ray r = {
        .origin = point_init(0, 1, -5),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, 5.0));
      assert(req(ig.xs[1].t, 5.0));
  }

  TEST {
      // A ray intersects a sphere (miss)
      ray r = {
        .origin = point_init(0, 2, -5),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 0);
  }

  TEST {
      // A ray intersects a sphere (inside)
      ray r = {
        .origin = point_init(0, 0, 0),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, -1.0));
      assert(req(ig.xs[1].t,  1.0));
  }

  TEST {
      // A ray intersects a sphere (behind)
      ray r = {
        .origin = point_init(0, 0, 5),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, -6.0));
      assert(req(ig.xs[1].t, -4.0));
      assert(ig.xs[0].o == &s);
      assert(ig.xs[1].o == &s);
  }

  TEST {
      // The hit, when all intersection_group have positive t
      object s = {0};
      sphere_init(&s);

      intersection_group ig = {
        .count = 2,
        .xs = {
          { .t = 1, .o = &s },
          { .t = 2, .o = &s },
        },
      };

      const intersection *hit = intersection_group_hit(&ig);

      assert(req(hit->t, 1.0));
      assert(hit->o == &s);
  }

  TEST {
      // The hit, when some intersection_group have negative t
      object s = {0};
      sphere_init(&s);

      intersection_group ig = {
        .count = 2,
        .xs = {
          { .t = -1, .o = &s },
          { .t = 1, .o = &s },
        },
      };

      const intersection *hit = intersection_group_hit(&ig);

      assert(req(hit->t, 1.0));
      assert(hit->o == &s);
  }

  TEST {
      // The hit, when all intersection_group have negative t
      object s = {0};
      sphere_init(&s);

      intersection_group ig = {
        .count = 2,
        .xs = {
          { .t = -2, .o = &s },
          { .t = -1, .o = &s },
        },
      };

      const intersection *hit = intersection_group_hit(&ig);

      assert(hit == NULL);
  }

  TEST {
      // The hit is always the lowest non-negative intersection
      object s = {0};
      sphere_init(&s);

      intersection_group ig = {
        .count = 4,
        .xs = {
          { .t = 5, .o = &s },
          { .t = 7, .o = &s },
          { .t = -3, .o = &s },
          { .t = 2, .o = &s },
        },
      };

      const intersection *hit = intersection_group_hit(&ig);

      assert(req(hit->t, 2.0));
      assert(hit->o == &s);
  }

  TEST {
      ray r = {
        .origin = point_init(1, 2, 3),
        .direction = vector_init(0, 1, 0),
      };

      m4 m = {0};
      translation(3, 4, 5, m);

      ray out = {0};
      ray_transform(&r, m, &out);

      assert(v4_eq(out.origin, point(4, 6, 8)));
      assert(v4_eq(out.direction, vector(0, 1, 0)));
  }

  TEST {
      ray r = {
        .origin = point_init(1, 2, 3),
        .direction = vector_init(0, 1, 0),
      };

      m4 m = {0};
      scaling(2, 3, 4, m);

      ray out = {0};
      ray_transform(&r, m, &out);

      assert(v4_eq(out.origin, point(2, 6, 12)));
      assert(v4_eq(out.direction, vector(0, 3, 0)));
  }

  TEST {
      // Sphere's default transform
      object o = {0};
      sphere_init(&o);

      assert(m4_eq(o.transform, IDENTITY));
  }

  TEST {
      // Set sphere's transform
      object o = {0};
      sphere_init(&o);

      m4 T = {0};
      translation(2, 3, 4, T);
      object_set_transform(&o, T);

      assert(m4_eq(o.transform, T));
  }

  TEST {
      // A ray intersects a scaled sphere
      ray r = {
        .origin = point_init(0, 0, -5),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);
      
      m4 T = {0};
      scaling(2, 2, 2, T);

      object_set_transform(&s, T);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, 3.0));
      assert(req(ig.xs[1].t, 7.0));
  }

  TEST {
      // A ray intersects a translated sphere
      ray r = {
        .origin = point_init(0, 0, -5),
        .direction = vector_init(0, 0, 1),
      };

      object s = {0};
      sphere_init(&s);
      
      m4 T = {0};
      translation(5, 0, 0, T);

      object_set_transform(&s, T);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      assert(ig.count == 0);
  }

  TEST {
      // The normal on a sphere at a point on the x axis
      object s = {0};
      sphere_init(&s);

      v4 n = {0};
      object_normal_at(&s, point(1, 0, 0), n);

      assert(v4_eq(n, vector(1, 0, 0)));
  }

  TEST {
      // The normal on a sphere at a point on the y axis
      object s = {0};
      sphere_init(&s);

      v4 n = {0};
      object_normal_at(&s, point(0, 1, 0), n);

      assert(v4_eq(n, vector(0, 1, 0)));
  }

  TEST {
      // The normal on a sphere at a point on the z axis
      object s = {0};
      sphere_init(&s);

      v4 n = {0};
      object_normal_at(&s, point(0, 0, 1), n);

      assert(v4_eq(n, vector(0, 0, 1)));
  }

  TEST {
      // The normal on a sphere at a point on a nonaxial point
      object s = {0};
      sphere_init(&s);

      v4 n = {0};
      object_normal_at(&s, point(ROOT_3_3, ROOT_3_3, ROOT_3_3), n);

      assert(v4_eq(n, vector(ROOT_3_3, ROOT_3_3, ROOT_3_3)));
  }

  TEST {
      // The normal on a sphere is normalized
      object s = {0};
      sphere_init(&s);

      v4 n = {0};
      object_normal_at(&s, point(ROOT_3_3, ROOT_3_3, ROOT_3_3), n);

      assert(v4_eq(n, vector(ROOT_3_3, ROOT_3_3, ROOT_3_3)));
      v4_norm(n, n);
      assert(v4_eq(n, vector(ROOT_3_3, ROOT_3_3, ROOT_3_3)));
  }

  TEST {
      // Computing the normal on a translated sphere
      object s = {0};
      sphere_init(&s);

      m4 T = {0};
      translation(0, 1, 0, T);

      object_set_transform(&s, T);

      v4 n = {0};
      object_normal_at(&s, point(0, 1.70711f, -0.70711f), n);

      assert(v4_eq(n, vector(0, 0.70711f, -0.70711f)));
  }

  TEST {
      // Computing the normal on a transformed sphere
      object s = {0};
      sphere_init(&s);

      m4 R = {0};
      rotation_z(PI_5, R);

      m4 S = {0};
      scaling(1, 0.5, 1, S);

      m4 Z = {0};
      m4_mul(S, R, Z);

      object_set_transform(&s, Z);

      v4 n = {0};
      object_normal_at(&s, point(0, ROOT_2_2, -ROOT_2_2), n);

      assert(v4_eq(n, vector(0, 0.97014f, -0.24254f)));
  }

  TEST {
      // A sphere has a default material
      object s = {0};
      sphere_init(&s);

      assert(v3_eq(s.material.color, color(1, 1, 1)));
      assert(req(s.material.ambient, 0.1));
      assert(req(s.material.diffuse, 0.9));
      assert(req(s.material.specular, 0.9));
      assert(req(s.material.shininess, 200.0));
  }

  TEST {
      // A sphere can be assigned a material
      object s = {0};
      sphere_init(&s);

      material m = {0};
      material_init(&m);
      m.ambient = 1.0;

      object_set_material(&s, &m);

      assert(req(s.material.ambient, 1.0));
  }

  TEST {
      // Precomputing intersection
      ray r = {
        .origin = point_init(0, 0, -5),
        .direction = vector_init(0, 0, 1),
      };

      object shape = {0};
      sphere_init(&shape);

      intersection i = {
        .t = 4,
        .o = &shape,
      };

      computations c = {0};
      computations_prepare(&i, &r, NULL, &c);

      assert(req(c.t, i.t));
      assert(c.o == i.o);
      assert(v4_eq(c.point, point(0, 0, -1)));
      assert(v4_eq(c.eyev, vector(0, 0, -1)));
      assert(v4_eq(c.normalv, vector(0, 0, -1)));
  }

  TEST {
      // Precomputing intersection outside
      ray r = {
        .origin = point_init(0, 0, -5),
        .direction = vector_init(0, 0, 1),
      };

      object shape = {0};
      sphere_init(&shape);

      intersection i = {
        .t = 4,
        .o = &shape,
      };

      computations c = {0};
      computations_prepare(&i, &r, NULL, &c);

      assert(c.inside == false);
  }

  TEST {
      // Precomputing intersection inside
      ray r = {
        .origin = point_init(0, 0, 0),
        .direction = vector_init(0, 0, 1),
      };

      object shape = {0};
      sphere_init(&shape);

      intersection i = {
        .t = 1,
        .o = &shape,
      };

      computations c = {0};
      computations_prepare(&i, &r, NULL, &c);

      assert(req(c.t, i.t));
      assert(c.o == i.o);
      assert(c.inside == true);
      assert(v4_eq(c.point, point(0, 0, 1)));
      assert(v4_eq(c.eyev, vector(0, 0, -1)));
      assert(v4_eq(c.normalv, vector(0, 0, -1)));
  }

  TEST {
      // Precomputing reflection vector
      ray r = {
        .origin = point_init(0, 1, -1),
        .direction = vector_init(0, -ROOT_2_2, ROOT_2_2),
      };

      object shape = {0};
      plane_init(&shape);

      intersection i = {
        .t = sqrt(2.0),
        .o = &shape,
      };

      computations c = {0};
      computations_prepare(&i, &r, NULL, &c);

      assert(v4_eq(c.reflectv, vector(0, ROOT_2_2, ROOT_2_2)));
  }
  
  TEST {
      // The hit should offset the point
      ray r = {
        .origin = point_init(0, 0, -5),
        .direction = vector_init(0, 0, 1),
      };

      object shape = {0};
      {
        sphere_init(&shape);

        m4 T = {0};
        translation(0, 0, 1, T);

        object_set_transform(&shape, T);
      }

      intersection i = {
        .t = 5,
        .o = &shape,
      };

      computations c = {0};
      computations_prepare(&i, &r, NULL, &c);

      assert(c.over_point[2] < (-(f64)EPSILON / 2.0));
      assert(c.point[2] > c.over_point[2]);
  }

  TEST {
    // The normal of a plane is constant everywhere
    object p = {0};
    plane_init(&p);

    {
      v4 n = {0};
      object_normal_at(&p, point(0, 0, 0), n);

      assert(v4_eq(n, vector(0, 1, 0)));
    }

    {
      v4 n = {0};
      object_normal_at(&p, point(10, 0, -10), n);

      assert(v4_eq(n, vector(0, 1, 0)));
    }

    {
      v4 n = {0};
      object_normal_at(&p, point(-5, 0, 150), n);

      assert(v4_eq(n, vector(0, 1, 0)));
    }
  }

  TEST {
    // Intersect with a ray parallel to the plane
    object p = {0};
    plane_init(&p);

    ray r = { 
      .origin = point_init(0, 10, 0),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {0};
    ray_intersect(&r, &p, &ig);

    assert(ig.count == 0);
  }

  TEST {
    // Intersect with a ray above a plane
    object p = {0};
    plane_init(&p);

    ray r = { 
      .origin = point_init(0, 1, 0),
      .direction = vector_init(0, -1, 0),
    };

    intersection_group ig = {0};
    ray_intersect(&r, &p, &ig);

    assert(ig.count == 1);
    assert(req(ig.xs[0].t, 1));
    assert(ig.xs[0].o == &p);
  }

  TEST {
    // Intersect with a ray below a plane
    object p = {0};
    plane_init(&p);

    ray r = { 
      .origin = point_init(0, -1, 0),
      .direction = vector_init(0, 1, 0),
    };

    intersection_group ig = {0};
    ray_intersect(&r, &p, &ig);

    assert(ig.count == 1);
    assert(req(ig.xs[0].t, 1));
    assert(ig.xs[0].o == &p);
  }

  TEST {
    // Finding n1 and n2 at various intersections
    object A = {0};
    {
      glass_sphere_init(&A);
      A.material.refractive_index = 1.5;

      m4 T = {0};
      scaling(2, 2, 2, T);
      object_set_transform(&A, T);
    }

    object B = {0};
    {
      glass_sphere_init(&B);
      B.material.refractive_index = 2.0;

      m4 T = {0};
      translation(0, 0, -0.25, T);
      object_set_transform(&B, T);
    }

    object C = {0};
    {
      glass_sphere_init(&C);
      C.material.refractive_index = 2.5;

      m4 T = {0};
      translation(0, 0, 0.25, T);
      object_set_transform(&C, T);
    }

    typedef struct {
      f64 t;
      const object *o;
      f64 n1;
      f64 n2;
    } test_case;
    test_case cases[] = {
      { 2, &A, 1.0, 1.5 },
      { 2.75, &B, 1.5, 2.0 },
      { 3.25, &C, 2.0, 2.5 },
      { 4.75, &B, 2.5, 2.5 },
      { 5.25, &C, 2.5, 1.5 },
      { 6, &A, 1.5, 1.0 },
    };
    u64 L = sizeof(cases) / sizeof(test_case);

    intersection_group ig = {0};
    for (u64 j = 0; j < L; j++) {
      test_case T = cases[j];
      ig.xs[ig.count++] = (intersection){ .t = T.t, .o = T.o };
    }

    ray r = {
      .origin = point_init(0, 0, -4),
      .direction = vector_init(0, 0, 1),
    };

    for (u64 i = 0; i < L; i++) {
      test_case T = cases[i];

      computations c = {0};
      computations_prepare(&ig.xs[i], &r, &ig, &c);

      assert(req(c.n1, T.n1));
      assert(req(c.n2, T.n2));
    }
  }

  TEST {
    // The under point is offset below the surface
    object shape = {0};
    {
      glass_sphere_init(&shape);

      m4 T = {0};
      translation(0, 0, 1, T);

      object_set_transform(&shape, T);
    }

    ray r = { 
      .origin = point_init(0, 0, -5),
      .direction = vector_init(0, 0, 1),
    };

    intersection i = {
      .t = 5,
      .o = &shape,
    };

    intersection_group ig = {
      .count = 1,
      .xs = { i },
    };

    computations c = {0};
    computations_prepare(&i, &r, &ig, &c);

    assert(c.under_point[2] > (EPSILON / 2.0));
    assert(c.point[2] < c.under_point[2]);
  }

  TEST {
    // schlick total internal reflection
    object shape = {0};
    glass_sphere_init(&shape);

    ray r = {
      .origin = point_init(0, 0, ROOT_2_2),
      .direction = vector_init(0, 1, 0),
    };

    intersection_group ig = {
      .count = 2,
      .xs = {
        { .t = -ROOT_2_2, .o = &shape, },
        { .t = ROOT_2_2, .o = &shape, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[1], &r, &ig, &c);

    f64 results = computations_schlick(&c);
    assert(req(results, 1.0));
  }

  TEST {
    // schlick perpendicular
    object shape = {0};
    glass_sphere_init(&shape);

    ray r = {
      .origin = point_init(0, 0, 0),
      .direction = vector_init(0, 1, 0),
    };

    intersection_group ig = {
      .count = 2,
      .xs = {
        { .t = -1, .o = &shape, },
        { .t = 1, .o = &shape, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[1], &r, &ig, &c);

    f64 results = computations_schlick(&c);
    assert(req(results, 0.04));
  }

  TEST {
    // schlick small angle
    object shape = {0};
    glass_sphere_init(&shape);

    ray r = {
      .origin = point_init(0, 0.99, -2),
      .direction = vector_init(0, 0, 1),
    };

    intersection_group ig = {
      .count = 1,
      .xs = {
        { .t = 1.8589, .o = &shape, },
      },
    };

    computations c = {0};
    computations_prepare(&ig.xs[0], &r, &ig, &c);

    f64 results = computations_schlick(&c);
    assert(req(results, 0.48873));
  }

  TEST {
    // cube intersect rays
    typedef struct {
      v4 origin;
      v4 direction;
      f64 t1;
      f64 t2;
    } test_case;
    test_case cases[] = {
      { point_init(5, 0.5, 0), vector_init(-1, 0, 0), 4, 6 },
      { point_init(-5, 0.5, 0), vector_init(1, 0, 0), 4, 6 },
      { point_init(0.5, 5, 0), vector_init(0, -1, 0), 4, 6 },
      { point_init(0.5, -5, 0), vector_init(0, 1, 0), 4, 6 },
      { point_init(0.5, 0, 5), vector_init(0, 0, -1), 4, 6 },
      { point_init(0.5, 0, -5), vector_init(0, 0, 1), 4, 6 },
      { point_init(0, 0.5, 0), vector_init(0, 0, 1), -1, 1 },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cube_init(&c);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      intersection_group ig = {0};
      ray_intersect(&r, &c, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, T.t1));
      assert(req(ig.xs[1].t, T.t2));
    }
  }

  TEST {
    // cube rays miss
    typedef struct {
      v4 origin;
      v4 direction;
    } test_case;
    test_case cases[] = {
      { point_init(-2, 0, 0), vector_init(0.2673, 0.5345, 0.8018) },
      { point_init(0, -2, 0), vector_init(0.8018, 0.2673, 0.5345) },
      { point_init(0, 0, -2), vector_init(0.5345, 0.8018, 0.2673) },
      { point_init(2, 0, 2), vector_init(0, 0, -1) },
      { point_init(0, 2, 2), vector_init(0, -1, 0) },
      { point_init(2, 2, 0), vector_init(-1, 0, 0) },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cube_init(&c);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      intersection_group ig = {0};
      ray_intersect(&r, &c, &ig);

      assert(ig.count == 0);
    }
  }

  TEST {
    // Cube normals
    typedef struct {
      v4 point;
      v4 normal;
    } test_case;
    test_case cases[] = {
      { point_init(1, 0.5, -0.8), vector_init(1, 0, 0) },
      { point_init(-1, -0.2, 0.9), vector_init(-1, 0, 0) },
      { point_init(-0.4, 1, -0.1), vector_init(0, 1, 0) },
      { point_init(0.3, -1, -0.7), vector_init(0, -1, 0) },
      { point_init(-0.6, 0.3, 1), vector_init(0, 0, 1) },
      { point_init(0.4, 0.4, -1), vector_init(0, 0, -1) },
      { point_init(1, 1, 1), vector_init(1, 0, 0) },
      { point_init(-1, -1, -1), vector_init(-1, 0, 0) },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cube_init(&c);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];

      v4 normal = {0};
      object_normal_at(&c, T.point, normal);

      assert(v4_eq(normal, T.normal));
    }
  }

  TEST {
    // A ray misses a cylinder
    typedef struct {
      v4 origin;
      v4 direction;
    } test_case;
    test_case cases[] = {
      { point_init(1, 0, 0), vector_init(0, 1, 0) },
      { point_init(0, 0, 0), vector_init(0, 1, 0) },
      { point_init(0, 0, -5), vector_init(1, 1, 1) },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object cyl = {0};
    cylinder_init(&cyl);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &cyl, &ig);

      assert(ig.count == 0);
    }
  }

  TEST {
    // A ray hits a cylinder
    typedef struct {
      v4 origin;
      v4 direction;
      f64 t0;
      f64 t1;
    } test_case;
    test_case cases[] = {
      { point_init(1, 0, -5), vector_init(0, 0, 1), 5, 5 },
      { point_init(0, 0, -5), vector_init(0, 0, 1), 4, 6 },
      { point_init(0.5, 0, -5), vector_init(0.1, 1, 1), 6.80798, 7.08872 },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object cyl = {0};
    cylinder_init(&cyl);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &cyl, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, T.t0));
      assert(req(ig.xs[1].t, T.t1));
    }
  }

  TEST {
    // Cylinder normals
    typedef struct {
      v4 point;
      v4 normal;
    } test_case;
    test_case cases[] = {
      { point_init(1, 0, 0), vector_init(1, 0, 0) },
      { point_init(0, 5, -1), vector_init(0, 0, -1) },
      { point_init(0, -2, 1), vector_init(0, 0, 1) },
      { point_init(-1, 1, 0), vector_init(-1, 0, 0) },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cylinder_init(&c);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];

      v4 normal = {0};
      object_normal_at(&c, T.point, normal);

      assert(v4_eq(normal, T.normal));
    }
  }

  TEST {
    // A ray hits a truncated cylinder
    typedef struct {
      v4 origin;
      v4 direction;
      f64 count;
    } test_case;
    test_case cases[] = {
      { point_init(0, 1.5, 0), vector_init(0.1, 1, 0), 0 },
      { point_init(0, 3, -5), vector_init(0, 0, 1), 0 },
      { point_init(0, 0, -5), vector_init(0, 0, 1), 0 },
      { point_init(0, 2, -5), vector_init(0, 0, 1), 0 },
      { point_init(0, 1, -5), vector_init(0, 0, 1), 0 },
      { point_init(0, 1.5, -2), vector_init(0, 0, 1), 2 },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object cyl = {0};
    cylinder_init(&cyl);
    cyl.value.cylinder.minimum = 1;
    cyl.value.cylinder.maximum = 2;

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &cyl, &ig);

      assert(ig.count == T.count);
    }
  }

  TEST {
    // A ray hits a capped cylinder
    typedef struct {
      v4 origin;
      v4 direction;
      f64 count;
    } test_case;
    test_case cases[] = {
      { point_init(0, 3, 0), vector_init(0, -1, 0), 2 },
      { point_init(0, 3, -2), vector_init(0, -1, 2), 2 },
      { point_init(0, 4, -2), vector_init(0, -1, 1), 2 },
      { point_init(0, 0, -2), vector_init(0, 1, 2), 2 },
      { point_init(0, -1, -2), vector_init(0, 1, 1), 2 },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object cyl = {0};
    cylinder_init(&cyl);
    cyl.value.cylinder.minimum = 1;
    cyl.value.cylinder.maximum = 2;
    cyl.value.cylinder.closed = true;

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &cyl, &ig);

      assert(ig.count == T.count);
    }
  }

  TEST {
    // Cylinder cap normals
    typedef struct {
      v4 point;
      v4 normal;
    } test_case;
    test_case cases[] = {
      { point_init(0, 1, 0), vector_init(0, -1, 0) },
      { point_init(0.5, 1, 0), vector_init(0, -1, 0) },
      { point_init(0, 1, 0.5), vector_init(0, -1, 0) },
      { point_init(0, 2, 0), vector_init(0, 1, 0) },
      { point_init(0.5, 2, 0), vector_init(0, 1, 0) },
      { point_init(0, 2, 0.5), vector_init(0, 1, 0) },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object cyl = {0};
    cylinder_init(&cyl);
    cyl.value.cylinder.minimum = 1;
    cyl.value.cylinder.maximum = 2;
    cyl.value.cylinder.closed = true;

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];

      v4 normal = {0};
      object_normal_at(&cyl, T.point, normal);

      assert(v4_eq(normal, T.normal));
    }
  }

  TEST {
    // A ray hits a cone
    typedef struct {
      v4 origin;
      v4 direction;
      f64 t0;
      f64 t1;
    } test_case;
    test_case cases[] = {
      { point_init(0, 0, -5), vector_init(0, 0, 1), 5, 5 },
      { point_init(0, 0, -5), vector_init(1, 1, 1), 8.66025, 8.66025 },
      { point_init(1, 1, -5), vector_init(-0.5, -1, 1), 4.55006, 49.44994 },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cone_init(&c);

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &c, &ig);

      assert(ig.count == 2);
      assert(req(ig.xs[0].t, T.t0));
      assert(req(ig.xs[1].t, T.t1));
    }
  }
  TEST {
    // A ray hits a capped cone parallel to one half
    object c = {0};
    cone_init(&c);

    ray r = {0};
    memcpy(r.origin, point(0, 0, -1), sizeof(v4));
    memcpy(r.direction, vector(0, 1, 1), sizeof(v4));

    v4_norm(r.direction, r.direction);

    intersection_group ig = {0};
    ray_intersect(&r, &c, &ig);

    assert(ig.count == 1);
    assert(req(ig.xs[0].t, 0.35355));
  }

  TEST {
    // A ray hits a capped cone
    typedef struct {
      v4 origin;
      v4 direction;
      u32 count;
    } test_case;
    test_case cases[] = {
      { point_init(0, 0, -5), vector_init(0, 1, 0), 0 },
      { point_init(0, 0, -0.25), vector_init(0, 1, 1), 2 },
      { point_init(0, 0, -0.25), vector_init(0, 1, 0), 4 },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cone_init(&c);
    c.value.cone.minimum = -0.5;
    c.value.cone.maximum = 0.5;
    c.value.cone.closed = true;

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];
      ray r = {0};
      memcpy(r.origin, T.origin, sizeof(v4));
      memcpy(r.direction, T.direction, sizeof(v4));

      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &c, &ig);

      assert(ig.count == T.count);
    }
  }

  TEST {
    // Cone normals
    typedef struct {
      v4 point;
      v4 normal;
    } test_case;
    test_case cases[] = {
      { point_init(0, 0, 0), vector_init(0, 0, 0) },
      { point_init(1, 1, 1), vector_init(0.5, -ROOT_2_2, 0.5) },
      { point_init(-1, -1, 0), vector_init(-ROOT_2_2, ROOT_2_2, 0) },
    };
    u32 L = sizeof(cases) / sizeof(test_case);

    object c = {0};
    cone_init(&c);
    /*
    c.value.cone.minimum = -0.5;
    c.value.cone.maximum = 0.5;
    c.value.cone.closed = true;
    */

    for (u32 i = 0; i < L; i++) {
      test_case T = cases[i];

      v4 normal = {0};
      object_normal_at(&c, T.point, normal);

      assert(v4_eq(normal, T.normal));
    }
  }
}


