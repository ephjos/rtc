#include "tests.h"

u32 test_total = 0;

void end_test_context(test_context *ctx)
{
  printf("[%3ld] %s passed\n", ctx->count, ctx->label);
}
