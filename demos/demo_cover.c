#include "demos.h"

const u32 W = 1000;
const u32 H = W;

void demo_cover()
{
  printf("-- demo cover\n");

  // Materials
  material white_material = {
    .color = color_init(1, 1, 1),
    .diffuse = 0.7,
    .ambient = 0.1,
    .specular = 0,
    .reflective = 0.1,
  };

  material blue_material = white_material;
  memcpy(blue_material.color, color(0.537, 0.831, 0.914), sizeof(vec4));

  material red_material = white_material;
  memcpy(red_material.color, color(0.941, 0.322, 0.388), sizeof(vec4));

  material purple_material = white_material;
  memcpy(purple_material.color, color(0.373, 0.404, 0.550), sizeof(vec4));

  // Transforms
  matrix4 standard_transform = {0};
  {
    matrix4 T = {0};
    translation(1, -1, 1, T);

    matrix4 S = {0};
    scaling(0.5, 0.5, 0.5, S);

    matrix4_mul(S, T, standard_transform);
  }

  matrix4 large_object = {0};
  {
    matrix4 S = {0};
    scaling(3.5, 3.5, 3.5, S);

    matrix4_mul(S, standard_transform, large_object);
  }

  matrix4 medium_object = {0};
  {
    matrix4 S = {0};
    scaling(3, 3, 3, S);

    matrix4_mul(S, standard_transform, medium_object);
  }

  matrix4 small_object = {0};
  {
    matrix4 S = {0};
    scaling(2, 2, 2, S);

    matrix4_mul(S, standard_transform, small_object);
  }

  // Objects
  object plane = {0};
  {
    plane_init(&plane);
    memcpy(plane.material.color, color(1, 1, 1), sizeof(vec4));
    plane.material.ambient = 1;
    plane.material.diffuse = 0;
    plane.material.specular = 0;

    matrix4 R = {0};
    rotation_x(PI_2, R);

    matrix4 T = {0};
    translation(0, 0, 500, T);

    matrix4 Z = {0};
    matrix4_mul(T, R, Z);

    object_set_transform(&plane, Z);
  }

  object sphere = {0};
  {
    sphere_init(&sphere);
    memcpy(sphere.material.color, color(0.373, 0.404, 0.550), sizeof(vec4));
    sphere.material.ambient = 0;
    sphere.material.diffuse = 0.2;
    sphere.material.specular = 1.0;
    sphere.material.shininess = 200.0;
    sphere.material.reflective = 0.7;
    sphere.material.transparency = 0.7;
    sphere.material.refractive_index = 1.5;

    object_set_transform(&sphere, large_object);
  }

  typedef struct {
    material *m;
    matrix4 *X;
    vec4 t;
  } cube_params;
  cube_params cubes[] = {
    { &white_material, &medium_object, vec4_init(4, 0, 0) },
    { &blue_material, &large_object, vec4_init(8.5, 1.5, -0.5) },
    { &red_material, &large_object, vec4_init(0, 0, 4) },
    { &white_material, &small_object, vec4_init(4, 0, 4) },
    { &purple_material, &medium_object, vec4_init(7.5, 0.5, 4) },
    { &white_material, &medium_object, vec4_init(-0.25, 0.25, 8) },
    { &blue_material, &large_object, vec4_init(4, 1, 7.5) },
    { &red_material, &medium_object, vec4_init(10, 2, 7.5) },
    { &white_material, &small_object, vec4_init(8, 2, 12) },
    { &white_material, &small_object, vec4_init(20, 1, 9) },
    { &blue_material, &large_object, vec4_init(-0.5, -5, 0.25) },
    { &red_material, &large_object, vec4_init(4, -4, 0) },
    { &white_material, &large_object, vec4_init(8.5, -4, 0) },
    { &white_material, &large_object, vec4_init(0, -4, 4) },
    { &purple_material, &large_object, vec4_init(-0.5, -4.5, 8) },
    { &white_material, &large_object, vec4_init(0, -8, 4) },
    { &white_material, &large_object, vec4_init(-0.5, -8.5, 8) },
  };
  u32 L = sizeof(cubes) / sizeof(cube_params);

  // Lights
  light main_light = {0};
  {
    point_light_init(&main_light, point4(50, 100, -50), color(1, 1, 1));
  }

  light optional_light = {0};
  {
    point_light_init(&optional_light, point4(-400, 50, -10), color(0.2, 0.2, 0.2));
  }

  // World
  world w = { 
    .objects_count = 2,
    .objects = {
      plane, sphere
    },
    .lights_count = 2,
    .lights = {
      main_light, optional_light,
    },
  };

  for (u32 i = 0; i < L; i++) {
    cube_params params = cubes[i];

    object cube_i = {0};
    {
      cube_init(&cube_i);
      object_set_material(&cube_i, params.m);

      matrix4 T = {0};
      translation(params.t[0], params.t[1], params.t[2], T);

      matrix4 Z = {0};
      matrix4_mul(T, *params.X, Z);
      object_set_transform(&cube_i, Z);
    }

    w.objects[w.objects_count++] = cube_i;
  }

  // Camera
  camera v = {0};
  {
    camera_init(&v, W, H, 0.785);

    matrix4 T = {0};
    view_transform(point4(-6, 6, -10), point4(6, 0, 6), vec4(-0.45, 1, 0), T);

    camera_set_transform(&v, T);
  }

  render_stats s = {0};
  canvas *c = camera_render(&v, &w, &s);

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen("./demo-out/demo_cover.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open demo-out/demo_cover.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote demo-out/demo_cover.ppm ");
    render_stats_print(&s);

    free(ppm);
    canvas_free(c);
  }
}

void render_and_save(world w, u32 frame, f64 step)
{
  // Camera
  camera v = {0};
  {
    camera_init(&v, W, H, 0.785);

    vec4 from = point4_init(-6, 6, -10);

    matrix4 R = {0};
    rotation_y(PI_3 * step, R);

    matrix4_mulv(R, from, from);

    matrix4 T = {0};
    view_transform(from, point4(6, 0, 6), vec4(-0.45, 1, 0), T);

    camera_set_transform(&v, T);
  }

  render_stats s = {0};
  canvas *c = camera_render(&v, &w, &s);

  char filepath[256] = {0};
  sprintf(filepath, "./demo-out/cover_%05ld.ppm", frame);

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) {
      fprintf(stderr, "Failed to open %s for writing", filepath);
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote %s ", filepath);
    render_stats_print(&s);

    free(ppm);
    canvas_free(c);
  }
}

/*
// Sweeps out a path over the scene, rendering many frames to be stitched
// externally
void demo_cover()
{
  printf("-- demo cover\n");

  // Materials
  material white_material = {
    .color = color_init(1, 1, 1),
    .diffuse = 0.7,
    .ambient = 0.1,
    .specular = 0,
    .reflective = 0.1,
  };

  material blue_material = white_material;
  memcpy(blue_material.color, color(0.537, 0.831, 0.914), sizeof(vec4));

  material red_material = white_material;
  memcpy(red_material.color, color(0.941, 0.322, 0.388), sizeof(vec4));

  material purple_material = white_material;
  memcpy(purple_material.color, color(0.373, 0.404, 0.550), sizeof(vec4));

  // Transforms
  matrix4 standard_transform = {0};
  {
    matrix4 T = {0};
    translation(1, -1, 1, T);

    matrix4 S = {0};
    scaling(0.5, 0.5, 0.5, S);

    matrix4_mul(S, T, standard_transform);
  }

  matrix4 large_object = {0};
  {
    matrix4 S = {0};
    scaling(3.5, 3.5, 3.5, S);

    matrix4_mul(S, standard_transform, large_object);
  }

  matrix4 medium_object = {0};
  {
    matrix4 S = {0};
    scaling(3, 3, 3, S);

    matrix4_mul(S, standard_transform, medium_object);
  }

  matrix4 small_object = {0};
  {
    matrix4 S = {0};
    scaling(2, 2, 2, S);

    matrix4_mul(S, standard_transform, small_object);
  }

  // Objects
  object plane = {0};
  {
    plane_init(&plane);
    memcpy(plane.material.color, color(1, 1, 1), sizeof(vec4));
    plane.material.ambient = 1;
    plane.material.diffuse = 0;
    plane.material.specular = 0;

    matrix4 R = {0};
    rotation_x(PI_2, R);

    matrix4 T = {0};
    translation(0, 0, 500, T);

    matrix4 Z = {0};
    matrix4_mul(T, R, Z);

    object_set_transform(&plane, Z);
  }

  object sphere = {0};
  {
    sphere_init(&sphere);
    memcpy(sphere.material.color, color(0.373, 0.404, 0.550), sizeof(vec4));
    sphere.material.ambient = 0;
    sphere.material.diffuse = 0.2;
    sphere.material.specular = 1.0;
    sphere.material.shininess = 200.0;
    sphere.material.reflective = 0.7;
    sphere.material.transparency = 0.7;
    sphere.material.refractive_index = 1.5;

    object_set_transform(&sphere, large_object);
  }

  typedef struct {
    material *m;
    matrix4 *X;
    vec4 t;
  } cube_params;
  cube_params cubes[] = {
    { &white_material, &medium_object, vec4_init(4, 0, 0) },
    { &blue_material, &large_object, vec4_init(8.5, 1.5, -0.5) },
    { &red_material, &large_object, vec4_init(0, 0, 4) },
    { &white_material, &small_object, vec4_init(4, 0, 4) },
    { &purple_material, &medium_object, vec4_init(7.5, 0.5, 4) },
    { &white_material, &medium_object, vec4_init(-0.25, 0.25, 8) },
    { &blue_material, &large_object, vec4_init(4, 1, 7.5) },
    { &red_material, &medium_object, vec4_init(10, 2, 7.5) },
    { &white_material, &small_object, vec4_init(8, 2, 12) },
    { &white_material, &small_object, vec4_init(20, 1, 9) },
    { &blue_material, &large_object, vec4_init(-0.5, -5, 0.25) },
    { &red_material, &large_object, vec4_init(4, -4, 0) },
    { &white_material, &large_object, vec4_init(8.5, -4, 0) },
    { &white_material, &large_object, vec4_init(0, -4, 4) },
    { &purple_material, &large_object, vec4_init(-0.5, -4.5, 8) },
    { &white_material, &large_object, vec4_init(0, -8, 4) },
    { &white_material, &large_object, vec4_init(-0.5, -8.5, 8) },
  };
  u32 L = sizeof(cubes) / sizeof(cube_params);

  // Lights
  light main_light = {0};
  {
    point_light_init(&main_light, point4(50, 100, -50), color(1, 1, 1));
  }

  light optional_light = {0};
  {
    point_light_init(&optional_light, point4(-400, 50, -10), color(0.2, 0.2, 0.2));
  }

  // World
  world w = { 
    .objects_count = 2,
    .objects = {
      plane, sphere
    },
    .lights_count = 2,
    .lights = {
      main_light, optional_light,
    },
  };

  for (u32 i = 0; i < L; i++) {
    cube_params params = cubes[i];

    object cube_i = {0};
    {
      cube_init(&cube_i);
      object_set_material(&cube_i, params.m);

      matrix4 T = {0};
      translation(params.t[0], params.t[1], params.t[2], T);

      matrix4 Z = {0};
      matrix4_mul(T, *params.X, Z);
      object_set_transform(&cube_i, Z);
    }

    w.objects[w.objects_count++] = cube_i;
  }

  u32 N = 120;
  for (u32 i = 0; i < N; i++) {
    f64 step = ((f64)i / (f64)N) - 0.5;
    render_and_save(w, i, step);
  }

}

*/
