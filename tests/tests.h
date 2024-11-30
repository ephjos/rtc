#ifndef __TESTS_H__
#define __TESTS_H__

#include "../src/rtc.h"

extern u32 test_total;
typedef struct {
  u32 count;
  const char *label;
} test_context;
void end_test_context(test_context *ctx);
#define TESTS() \
  test_context __test_context__ __attribute__((cleanup(end_test_context))) = {\
    .label = __func__,\
    .count = 0,\
  };

#define TEST __test_context__.count++; test_total++;

void test_camera();
void test_canvas();
void test_lights();
void test_materials();
void test_matrix();
void test_objects();
void test_patterns();
void test_primitives();
void test_transform();
void test_world();

#endif
