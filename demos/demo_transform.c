#include "demos.h"



#define TRANSFORM_DEMO_BLOCK(s, x, y) \
  for (s32 i = -s; i < s; i++) {\
    for (s32 j = -s; j < s; j++) {\
      canvas_safe_write(c, (s32)x+i, (s32)y+j, white);\
    }\
  }

void demo_transform()
{
  printf("-- demo transform\n");

  const u32 w = 300;
  const f64 clock_w = (f64)w * (3.0/8.0);
  const f64 center = (f64)w/2.0;
  canvas *c = canvas_alloc(w, w);
  vec4 white = color_init(1, 1, 1);

  matrix4 T = {0};
  translation(center, center, 0, T);

  matrix4 S = {0};
  scaling(clock_w, clock_w, clock_w, S);

  matrix4 R = {0};
  matrix4 Z = {0};

  vec4 q = {0};

  for (u32 k = 0; k < 12; k++) {
    vec4 p = point4_init(1, 0, 0);

    // Get this points rotation
    rotation_z((2 * PI) * ((f64)k / 12.0), R);

    // Combine transformations
    matrix4_mul(S, R, Z);
    matrix4_mul(T, Z, Z);

    // Apply transformations
    matrix4_mulv(Z, p, q);

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
