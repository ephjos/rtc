
#define LR_IMPLEMENTATION
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>

#include "canvas.h"
#include "transform.h"
#include "vec4.h"


int main(int argc, char **argv)
{
	LR_PRELUDE(argc, argv);

	//vec4_demo();
	canvas_demo();
	transform_demo();

	return 0;
}
