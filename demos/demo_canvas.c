#include "demos.h"


typedef struct {
  v4 position;
  v4 velocity;
} canvas_projectile;

typedef struct {
  v4 gravity;
  v4 wind;
} canvas_environment;

void canvas_tick(canvas_environment *e, canvas_projectile *p)
{
  v4_add(p->position, p->velocity, p->position);
  v4 env_comp = {0};
  v4_add(e->gravity, e->wind, env_comp);
  v4_add(p->velocity, env_comp, p->velocity);
}

#define CANVAS_DEMO_BLOCK(s) \
  for (s32 i = -s; i < s; i++) {\
    for (s32 j = -s; j < s; j++) {\
      canvas_safe_write(c, (s32)p.position[0]+i, c->height - (s32)p.position[1]+j, red);\
    }\
  }

void demo_canvas(void)
{
  printf("-- demo canvas\n");

  canvas *c = canvas_alloc(900, 550);
  v3 red = color_init(1, 0, 0);

  canvas_projectile p = {
    .position = point_init(0, 1, 0),
    .velocity = vector_init(1, 1.8, 0),
  };
  v4_norm(p.velocity, p.velocity);
  v4_scale(p.velocity, 11.25, p.velocity);

  canvas_environment e = {
    .gravity = vector_init(0, -0.1, 0),
    .wind = vector_init(-0.01, 0, 0),
  };

  CANVAS_DEMO_BLOCK(3);
  while (p.position[1] > 0) {
    canvas_tick(&e, &p);
    CANVAS_DEMO_BLOCK(3);
  }

  char *ppm = canvas_to_ppm(c);

  FILE *fp = fopen("./demo-out/demo_canvas.ppm", "w");
  if (fp == NULL) {
    perror("Failed to open demo-out/demo_canvas.ppm for writing");
    free(ppm);
    canvas_free(c);
    return;
  }

  fprintf(fp, "%s", ppm);
  fclose(fp);

  printf("wrote demo-out/demo_canvas.ppm\n");

  free(ppm);
  canvas_free(c);
}
