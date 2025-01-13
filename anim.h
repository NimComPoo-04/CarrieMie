#ifndef _ANIMATE_H_
#define _ANIMATE_H_

#include <raylib.h>
#define MAX_FRAMES 10

// Animation gets the rectangle for animation

typedef struct
{
	float fps;				// frames per second
	int frames_count;			// frame counter
	Rectangle frame_source[MAX_FRAMES];	// frame sources
} anim_t;

Rectangle anim_get_frame(anim_t *a, float epoch, int start_frame);

#endif
