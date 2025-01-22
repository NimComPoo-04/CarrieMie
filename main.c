#include <raylib.h>
#include <raymath.h>

#include "res.h"
#include "anim.h"
#include "map.h"
#include "player.h"
#include "pathfinder.h"
#include "test.h"

struct cell_t queue_delete(queue_t *q);
int main(void)
{
	InitWindow(0, 0, "...");
	SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	ToggleFullscreen();
	SetTargetFPS(60);

	res_t *resources = res_load();

	map_t map = map_create(40, 40);

	player_t plr = {
		.pos = {2, 2},
		.vel = {10, 10},
		.eye = 0.1,
		.size = 0.5
	};

	path_t path = { &map, {0}, 0};

	path_create(&path);

	for(int i = 0; i < 10; i++)
		queue_insert(&path.queue, i, 0, 0);
	queue_dump(&path.queue);
	for(int i = 0; i < 10; i++)
		TraceLog(LOG_INFO, "%d", queue_delete(&path.queue).x);
  
	int which = 5;

	while(!WindowShouldClose())
	{
		BeginDrawing();

		if(IsKeyPressed(KEY_TAB))
			which = (which + 1) % 8;

		switch(which)
		{
			case 0: test1_texture_rendering(resources); break;
			case 1: test2_map_generation(resources, &map); break;
			case 2: test2_map_rect(&map); break;
			case 3: test2_map_bsp(&map); break;
			case 4: test3_player_and_test(&plr, &map, resources); break;
			case 5: test4_player2d_and_3d(&plr, &map, resources); break;
			case 6: test4_player_3d(&plr, &map, resources); break;
			case 7: test5_pathfinder(&plr, &path, resources); break;
		}


		EndDrawing();
	}

	map_destroy(&map);
	res_unload(resources);
	CloseWindow();

	path_destroy(&path);

	return 0;
}
