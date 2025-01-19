#ifndef _TEST_H_
#define _TEST_H_

#include <raylib.h>
#include <raymath.h>

#include "res.h"
#include "anim.h"
#include "map.h"
#include "player.h"
#include "render.h"
#include "ray.h"

#include "util.h"

void test1_texture_rendering(res_t *resources);
void test2_map_generation(res_t *resources, map_t *m);
void test2_map_rect(map_t *m);
void test2_map_bsp(map_t *m);
void test3_player_and_test(player_t *p, map_t *m, res_t *resources);
void test4_player2d_and_3d(player_t *p, map_t *m, res_t *resources);

#endif
