#include "tests.h"

int main(int argc, char **argv)
{
  test_primitives();
  test_canvas();
  test_matrix();
  test_transform();
  test_objects();
  test_lights();
  test_materials();
  test_world();
  test_camera();
  test_patterns();

  printf("\n%ld total tests passed\n", test_total);
  return 0;
}
