#include "demos.h"


void demo_patterns()
{
  printf("-- demo patterns\n");

  pattern checker = {0};
  {
    checker_pattern_init(&checker, color(1, 1, 1), color(0, 0, 0));

    m4 S = {0};
    scaling(0.4, 0.4, 0.4, S);

    pattern_set_transform(&checker, S);
  }

  pattern ring = {0};
  {
    ring_pattern_init(&ring, color(0.6, 0.6, 0), color(1, 1, 0));

    m4 RZ = {0};
    rotation_z(PI_4, RZ);

    m4 RY = {0};
    rotation_x(PI_6, RY);

    m4 S = {0};
    scaling(0.4, 0.4, 0.8, S);

    m4 Z = {0};
    m4_mul(RY, RZ, Z);
    m4_mul(S, Z, Z);

    pattern_set_transform(&ring, Z);
  }

  pattern striped = {0};
  {
    striped_pattern_init(&striped, color(0, 0.4, 0), color(0.1, 0.8, 0.1));

    m4 R = {0};
    rotation_y(PI_6, R);

    m4 S = {0};
    scaling(0.2, 0.2, 0.2, S);

    m4 Z = {0};
    m4_mul(S, R, Z);

    pattern_set_transform(&striped, Z);
  }

  pattern gradient = {0};
  {
    gradient_pattern_init(&gradient, color(1, 0.2, 0.4), color(0.2, 0.4, 1));

    m4 S = {0};
    scaling(2, 2, 2, S);

    m4 T = {0};
    translation(3, 0, 0, T);

    m4 Z = {0};
    m4_mul(T, S, Z);

    pattern_set_transform(&gradient, Z);
  }

  object floor = {0};
  {
    plane_init(&floor);

    material m = {0};
    material_init(&m);
    m.p = &checker;
    m.reflective = 0.1;

    memcpy(m.color, color(1, 0.9, 0.9), sizeof(v3));
    m.specular = 0.0;

    object_set_material(&floor, &m);
  }

  object water = {0};
  {
    plane_init(&water);

    m4 T = {0};
    translation(0, 0.4, 0, T);

    object_set_transform(&water, T);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(0.1, 0.6, 0.85), sizeof(v3));
    m.transparency = 0.75;
    m.reflective = 0.5;
    m.refractive_index = REFRACT_WATER;

    object_set_material(&water, &m);
  }

  object middle = {0};
  {
    sphere_init(&middle);

    m4 T = {0};
    translation(-0.5, 1, 0.5, T);

    object_set_transform(&middle, T);

    material m = {0};
    material_init(&m);
    m.p = &ring;

    memcpy(m.color, color(0.1, 1, 0.5), sizeof(v3));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&middle, &m);
  }

  object right = {0};
  {
    cube_init(&right);

    m4 T = {0};
    translation(1.5, 0.5, -0.5, T);

    m4 S = {0};
    scaling(0.5, 0.5, 0.5, S);

    m4 Z = {0};
    m4_mul(T, S, Z);

    object_set_transform(&right, Z);

    material m = {0};
    material_init(&m);
    m.p = &gradient;

    memcpy(m.color, color(0.5, 1, 0.1), sizeof(v3));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&right, &m);
  }

  object left = {0};
  {
    sphere_init(&left);

    m4 T = {0};
    translation(-1.5, 0.33f, -0.75f, T);

    m4 S = {0};
    scaling(0.33f, 0.33f, 0.33f, S);

    m4 Z = {0};
    m4_mul(T, S, Z);

    object_set_transform(&left, Z);

    material m = {0};
    material_init(&m);
    m.p = &striped;

    memcpy(m.color, color(1, 0.8, 0.1), sizeof(v3));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&left, &m);
  }

  light l = {0};
  {
    point_light_init(&l, point(-10, 10, -10), color(1, 1, 1));
  }

  camera v = {0};
  {
    camera_init(&v, DEMO_W, DEMO_H, PI_3);

    m4 T = {0};
    view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0), T);

    camera_set_transform(&v, T);
  }

  world w = { 
    .objects_count = 5,
    .objects = {
      floor, water, middle, right, left,
    },
    .lights_count = 1,
    .lights = {
      l
    },
  };

  render_stats s = {0};
  canvas *c = camera_render(&v, &w, &s);

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen("./demo-out/demo_patterns.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open demo-out/demo_patterns.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote demo-out/demo_patterns.ppm ");
    render_stats_print(&s);

    free(ppm);
    canvas_free(c);
  }
}
