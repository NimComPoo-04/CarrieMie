#include <raylib.h>
#include "res.h"

res_t *res_load()
{
	res_t *r = MemAlloc(sizeof(res_t));

	r->atlas = LoadTexture("assets/test.png");

	// The top wall animation image
	r->source[RES_TOP_WALL].fps = 5;
	r->source[RES_TOP_WALL].frames_count = 5;
	r->source[RES_TOP_WALL].frame_source[0] = (Rectangle){0,   0, 32, 32};
	r->source[RES_TOP_WALL].frame_source[1] = (Rectangle){32,  0, 32, 32};
	r->source[RES_TOP_WALL].frame_source[2] = (Rectangle){64,  0, 32, 32};
	r->source[RES_TOP_WALL].frame_source[3] = (Rectangle){96,  0, 32, 32};
	r->source[RES_TOP_WALL].frame_source[4] = (Rectangle){128, 0, 32, 32};

	// The top floor view animations
	r->source[RES_TOP_FLOOR].fps = 3;
	r->source[RES_TOP_FLOOR].frames_count = 5;
	r->source[RES_TOP_FLOOR].frame_source[0] = (Rectangle){0,   32, 32, 32};
	r->source[RES_TOP_FLOOR].frame_source[1] = (Rectangle){32,  32, 32, 32};
	r->source[RES_TOP_FLOOR].frame_source[2] = (Rectangle){64,  32, 32, 32};
	r->source[RES_TOP_FLOOR].frame_source[3] = (Rectangle){96,  32, 32, 32};
	r->source[RES_TOP_FLOOR].frame_source[4] = (Rectangle){128, 32, 32, 32};

	return r;
}

void res_unload(res_t *r)
{
	MemFree(r);
}
