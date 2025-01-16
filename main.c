#include <raylib.h>
#include <raymath.h>

#include "res.h"
#include "anim.h"
#include "map.h"

void test1_texture_rendering(res_t *resources);
void test2_map_generation(res_t *res, map_t *m);
void test2_map_bsp(map_t *p);

int main(void)
{
	InitWindow(0, 0, "...");
	SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	ToggleFullscreen();

	res_t *resources = res_load();

	map_t map = map_create(40, 40);

	while(!WindowShouldClose())
	{
		BeginDrawing();

		//test1_texture_rendering(resources);
		test2_map_generation(resources, &map);
		//test2_map_bsp(&map);
		//test2_map_rect(&map);

		EndDrawing();
	}

	map_destroy(&map);
	res_unload(resources);
	CloseWindow();

	return 0;
}
