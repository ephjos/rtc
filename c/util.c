
#include <math.h>
#include <stdbool.h>

#include "util.h"

bool req(float a, float b) {
	return fabs(a - b) < 0.00001;
}
