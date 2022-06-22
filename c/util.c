
#include <math.h>
#include <stdbool.h>

#include "util.h"

bool req(double a, double b) {
	return fabs(a - b) < EPSILON;
}
