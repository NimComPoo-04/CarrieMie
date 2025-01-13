#include "anim.h"
#include <raylib.h>

Rectangle anim_get_frame(anim_t *a, float epoch, int start_frame)
{
	// (frames per second * time + (start frame = 0)) %  = actual frame

	float dt = GetTime() - epoch;
	int frame = ((int)(a->fps * dt + start_frame)) % a->frames_count;

	return a->frame_source[frame];
}
