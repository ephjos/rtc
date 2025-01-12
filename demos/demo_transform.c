#include "demos.h"



#define TRANSFORM_DEMO_BLOCK(s, x, y) \
  for (s32 i = -s; i < s; i++) {\
    for (s32 j = -s; j < s; j++) {\
      canvas_safe_write(c, (s32)x+i, (s32)y+j, white);\
    }\
  }

void demo_transform(void)
{
  printf("-- demo transform\n");

  const u32 w = 300;
  const f64 clock_w = (f64)w * (3.0/8.0);
  const f64 center = (f64)w/2.0;
  canvas *c = canvas_alloc(w, w);
  v3 white = color_init(1, 1, 1);

  m4 T = {0};
  translation(center, center, 0, T);

  m4 S = {0};
  scaling(clock_w, clock_w, clock_w, S);

  m4 R = {0};
  m4 Z = {0};

  v4 q = {0};

  for (u32 k = 0; k < 12; k++) {
    v4 p = point_init(1, 0, 0);

    // Get this points rotation
    rotation_z((2 * PI) * ((f64)k / 12.0), R);

    // Combine transformations
    m4_mul(S, R, Z);
    m4_mul(T, Z, Z);

    // Apply transformations
    m4_mulv(Z, p, q);

    TRANSFORM_DEMO_BLOCK(3, q[0], q[1]);
  }

  {
    char *ppm = canvas_to_ppm(c);
    FILE *fp = fopen("./demo-out/demo_transform.ppm", "w");
    if (fp == NULL) {
      perror("Failed to open demo-out/demo_transform.ppm for writing");
      free(ppm);
      canvas_free(c);
      return;
    }

    fprintf(fp, "%s", ppm);
    fclose(fp);

    printf("wrote demo-out/demo_transform.ppm\n");

    free(ppm);
    canvas_free(c);
  }
}
