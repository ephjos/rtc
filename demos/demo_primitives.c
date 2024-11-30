#include "demos.h"
  

typedef struct primitives_projectile {
  vec4 position;
  vec4 velocity;
} primitives_projectile;

typedef struct primitives_environment {
  vec4 gravity;
  vec4 wind;
} primitives_environment;

void primitives_tick(primitives_environment *e, primitives_projectile *p)
{
  vec4_add(p->position, p->velocity, p->position);
  vec4 env_comp = {0};
  vec4_add(e->gravity, e->wind, env_comp);
  vec4_add(p->velocity, env_comp, p->velocity);
}

void demo_primitives()
{
  printf("-- demo primitives\n");

  primitives_projectile p = {
    .position = point4_init(0, 1, 0),
    .velocity = vec4_init(1, 1, 0),
  };
  vec4_norm(p.velocity, p.velocity);

  primitives_environment e = {
    .gravity = vec4_init(0, -0.1, 0),
    .wind = vec4_init(-0.01f, 0, 0),
  };

  while (p.position[1] > 0) {
    //vec4_print(p.position);
    primitives_tick(&e, &p);
  }
  vec4_print(p.position);
}
