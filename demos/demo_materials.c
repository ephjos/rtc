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

  memcpy(s.material.color, color(1, 0.2, 1), sizeof(vec4));

  light l = {0};
  point_light_init(&l, point4(-10, 10, -10), color(1, 1, 1));

  ray r = {
    .origin = point4_init(0, 0, -5),
  };

  for (u32 i = 0; i < w; i++) {
    f64 world_y = half - pixel_size * (f64)i;
    for (u32 j = 0; j < w; j++) {
      f64 world_x = -half + pixel_size * (f64)j;

      vec4 target = point4_init((f64)world_x, (f64)world_y, (f64)wall_z);
      vec4_sub(target, r.origin, r.direction);
      vec4_norm(r.direction, r.direction);

      intersection_group ig = {0};
      ray_intersect(&r, &s, &ig);

      const intersection *hit = intersection_group_hit(&ig);
      if (hit != NULL) {
        vec4 p = {0};
        ray_position(&r, hit->t, p);

        vec4 normal = {0};
        object_normal_at(&s, p, normal);

        vec4 eye = {0};
        vec4_neg(r.direction, eye);

        vec4 px = {0};
        material_lighting(&s.material, &l, hit->o, p, eye, normal, false, px);

        canvas_write(c, i, j, px);
      }
    }
  }

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen("./images/demo_materials.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open images/demo_materials.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote images/demo_materials.ppm\n");

    free(ppm);
    canvas_free(c);
  }
}
