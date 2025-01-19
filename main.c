#include <raylib.h>
#include <raymath.h>

#include "res.h"
#include "anim.h"
#include "map.h"
#include "player.h"

#include "test.h"

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
  
	int which = 5;

	while(!WindowShouldClose())
	{
		BeginDrawing();

		if(IsKeyPressed(KEY_TAB))
			which = (which + 1) % 7;

		switch(which)
		{
			case 0: test1_texture_rendering(resources); break;
			case 1: test2_map_generation(resources, &map); break;
			case 2: test2_map_rect(&map); break;
			case 3: test2_map_bsp(&map); break;
			case 4: test3_player_and_test(&plr, &map, resources); break;
			case 5: test4_player2d_and_3d(&plr, &map, resources); break;
			case 6: test4_player_3d(&plr, &map, resources); break;
		}


		EndDrawing();
	}

	map_destroy(&map);
	res_unload(resources);
	CloseWindow();

	return 0;
}
