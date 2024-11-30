#include "demos.h"

void demo_plane()
{
  printf("-- demo plane\n");

  object floor = {0};
  {
    plane_init(&floor);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(1, 0.9, 0.9), sizeof(vec4));
    m.specular = 0.0;

    object_set_material(&floor, &m);
  }

  object middle = {0};
  {
    sphere_init(&middle);

    matrix4 T = {0};
    translation(-0.5, 1, 0.5, T);

    object_set_transform(&middle, T);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(0.1, 1, 0.5), sizeof(vec4));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&middle, &m);
  }

  object right = {0};
  {
    sphere_init(&right);

    matrix4 T = {0};
    translation(1.5, 0.5, -0.5, T);

    matrix4 S = {0};
    scaling(0.5, 0.5, 0.5, S);

    matrix4 Z = {0};
    matrix4_mul(T, S, Z);

    object_set_transform(&right, Z);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(0.5, 1, 0.1), sizeof(vec4));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&right, &m);
  }

  object left = {0};
  {
    sphere_init(&left);

    matrix4 T = {0};
    translation(-1.5, 0.33f, -0.75f, T);

    matrix4 S = {0};
    scaling(0.33f, 0.33f, 0.33f, S);

    matrix4 Z = {0};
    matrix4_mul(T, S, Z);

    object_set_transform(&left, Z);

    material m = {0};
    material_init(&m);

    memcpy(m.color, color(1, 0.8, 0.1), sizeof(vec4));
    m.diffuse = 0.7;
    m.specular = 0.3;

    object_set_material(&left, &m);
  }

  light l = {0};
  {
    point_light_init(&l, point4(-10, 10, -10), color(1, 1, 1));
  }

  camera v = {0};
  {
    camera_init(&v, DEMO_W, DEMO_H, PI_3);

    matrix4 T = {0};
    view_transform(point4(0, 1.5, -5), point4(0, 1, 0), vec4(0, 1, 0), T);

    camera_set_transform(&v, T);
  }

  world w = { 
    .objects_count = 4,
    .objects = {
      floor, middle, right, left,
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
    FILE *fp = fopen("./images/demo_plane.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open images/demo_plane.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote images/demo_plane.ppm ");
    render_stats_print(&s);

    free(ppm);
    canvas_free(c);
  }
}
