#pragma once
#include <stdbool.h>

bool req(double a, double b);

#define EPSILON (0.00001)

#undef MAX
#define MAX( a, b ) ((a) > (b) ? (a) : (b))

#undef MIN
#define MIN( a, b ) ((a) > (b) ? (b) : (a))

#undef CLAMP
#define CLAMP( x, a, b ) (MIN(MAX(x, a), b))
