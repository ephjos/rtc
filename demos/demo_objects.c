#include "demos.h"


void demo_objects() 
{
  printf("-- demo objects\n");

  const u32 w = 300;
  const u32 wall_z = 10;
  const u32 wall_size = 7;
  const f64 pixel_size = (f64)wall_size / (f64)w;
  const f64 half = (f64)wall_size/2.0;
  canvas *c = canvas_alloc(w, w);
  v3 red = color_init(1, 0, 0);

  object s = {0};
  sphere_init(&s);

  ray r = {
    .origin = point_init(0, 0, -5),
  };

  for (u32 i = 0; i < w; i++) {
    f64 world_y = half - pixel_size * (f64)i;
    for (u32 j = 0; j < w; j++) {
      f64 world_x = -half + pixel_size * (f64)j;

      v4 target = point_init((f64)world_x, (f64)world_y, (f64)wall_z);
      v4_sub(target, r.origin, r.direction);
      v4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      if (ig.count > 0) {
        canvas_write(c, j, i, red);
      }
    }
  }

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen("./demo-out/demo_objects.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open demo-out/demo_objects.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote demo-out/demo_objects.ppm\n");

    free(ppm);
    canvas_free(c);
  }
}

