#include "demos.h"


void demo_materials()
{
  printf("-- demo materials\n");

  const u32 w = DEMO_W;
  const u32 wall_z = 10;
  const u32 wall_size = 7;
  const f64 pixel_size = (f64)wall_size / (f64)w;
  const f64 half = (f64)wall_size/2.0;
  canvas *c = canvas_alloc(w, w);

  object s = {0};
  sphere_init(&s);

  memcpy(s.material.color, color(1, 0.2, 1), sizeof(v3));

  light l = {0};
  point_light_init(&l, point(-10, 10, -10), color(1, 1, 1));

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

      const intersection *hit = intersection_group_hit(&ig);
      if (hit != NULL) {
        v4 p = {0};
        ray_position(&r, hit->t, p);

        v4 normal = {0};
        object_normal_at(&s, p, normal);

        v4 eye = {0};
        v4_neg(r.direction, eye);

        v3 px = {0};
        material_lighting(&s.material, &l, hit->o, p, eye, normal, false, px);

        canvas_write(c, i, j, px);
      }
    }
  }

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen("./demo-out/demo_materials.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open demo-out/demo_materials.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote demo-out/demo_materials.ppm\n");

    free(ppm);
    canvas_free(c);
  }
}
