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

void test_camera(void);
void test_canvas(void);
void test_lights(void);
void test_materials(void);
void test_matrix(void);
void test_objects(void);
void test_patterns(void);
void test_primitives(void);
void test_transform(void);
void test_world(void);

#endif
