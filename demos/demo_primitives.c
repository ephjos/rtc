#include "demos.h"
  

typedef struct primitives_projectile {
  v4 position;
  v4 velocity;
} primitives_projectile;

typedef struct primitives_environment {
  v4 gravity;
  v4 wind;
} primitives_environment;

void primitives_tick(primitives_environment *e, primitives_projectile *p)
{
  v4_add(p->position, p->velocity, p->position);
  v4 env_comp = {0};
  v4_add(e->gravity, e->wind, env_comp);
  v4_add(p->velocity, env_comp, p->velocity);
}

void demo_primitives()
{
  printf("-- demo primitives\n");

  primitives_projectile p = {
    .position = point_init(0, 1, 0),
    .velocity = vector_init(1, 1, 0),
  };
  v4_norm(p.velocity, p.velocity);

  primitives_environment e = {
    .gravity = vector_init(0, -0.1, 0),
    .wind = vector_init(-0.01f, 0, 0),
  };

  while (p.position[1] > 0) {
    //v4_print(p.position);
    primitives_tick(&e, &p);
  }
  v4_print(p.position);
}
