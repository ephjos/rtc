
#include <math.h>
#include <stdbool.h>

#include "rtc.h"

bool req(double a, double b) {
	return fabs(a - b) < EPSILON;
}
