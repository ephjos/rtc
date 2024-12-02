#include "demos.h"

void demo_camera()
{
  printf("-- demo camera\n");

  object floor = {0};
  {
    sphere_init(&floor);

    m4 T = {0};
    scaling(10, 0.01f, 10, T);

    object_set_transform(&floor, T);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(1, 0.9, 0.9), sizeof(v3));
    m.specular = 0.0;

    object_set_material(&floor, &m);
  }

  object left_wall = {0};
  {
    sphere_init(&left_wall);

    m4 S = {0};
    scaling(10, 0.01f, 10, S);

    m4 RX = {0};
    rotation_x(PI_2, RX);

    m4 RY = {0};
    rotation_y(-PI_4, RY);

    m4 T = {0};
    translation(0, 0, 5, T);

    m4 Z = {0};
    m4_mul(RX, S, Z);
    m4_mul(RY, Z, Z);
    m4_mul(T, Z, Z);

    object_set_transform(&left_wall, Z);

    object_set_material(&left_wall, &floor.material);
  }

  object right_wall = {0};
  {
    sphere_init(&right_wall);

    m4 S = {0};
    scaling(10, 0.01f, 10, S);

    m4 RX = {0};
    rotation_x(PI_2, RX);

    m4 RY = {0};
    rotation_y(PI_4, RY);

    m4 T = {0};
    translation(0, 0, 5, T);

    m4 Z = {0};
    m4_mul(RX, S, Z);
    m4_mul(RY, Z, Z);
    m4_mul(T, Z, Z);

    object_set_transform(&right_wall, Z);

    object_set_material(&right_wall, &floor.material);
  }

  object middle = {0};
  {
    sphere_init(&middle);

    m4 T = {0};
    translation(-0.5, 1, 0.5, T);

    object_set_transform(&middle, T);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(0.1, 1, 0.5), sizeof(v3));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&middle, &m);
  }

  object right = {0};
  {
    sphere_init(&right);

    m4 T = {0};
    translation(1.5, 0.5, -0.5, T);

    m4 S = {0};
    scaling(0.5, 0.5, 0.5, S);

    m4 Z = {0};
    m4_mul(T, S, Z);

    object_set_transform(&right, Z);

    material m = {0};
    material_init(&m);

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
    .objects_count = 6,
    .objects = {
      floor, left_wall, right_wall, middle, right, left,
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
    FILE *fp = fopen("./demo-out/demo_camera.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open demo-out/demo_camera.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote demo-out/demo_camera.ppm ");
    render_stats_print(&s);

    free(ppm);
    canvas_free(c);
  }
}

