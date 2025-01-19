#ifndef _RENDER_H_
#define _RENDER_H_

#include <raylib.h>
#include <raymath.h>

#include "player.h"
#include "map.h"

void render_player_view(player_t *p, map_t *m);

#endif
