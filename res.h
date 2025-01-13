#ifndef _RES_H_
#define _RES_H_

#include <raylib.h>
#include "anim.h"

typedef enum ResType
{
	RES_TOP_WALL,
	RES_TOP_FLOOR,
	RES_COUNT
} ResType;

typedef struct res_t
{
	Texture2D atlas;
	anim_t source[RES_COUNT];
} res_t;

// load resources for the game
res_t *res_load();
void res_unload(res_t *);

#endif 
