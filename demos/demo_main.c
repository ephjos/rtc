#include "demos.h"

int main(int argc, char **argv)
{
  srand((unsigned)time(NULL));

  if (argc == 1) {
    demo_primitives();
    demo_canvas();
    demo_matrix();
    demo_transform();
    demo_objects();
    demo_lights();
    demo_materials();
    demo_world();
    demo_camera();
    demo_plane();
    demo_patterns();
  } else {
    demo_cover();
  }
  return 0;
}
