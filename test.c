#include <raylib.h>
#include <raymath.h>

#include "res.h"
#include "anim.h"
#include "map.h"

#include "util.h"

void test1_texture_rendering(res_t *resources)
{
		ClearBackground(DARKBLUE);

		DrawTexture(resources->atlas, 0, 0, WHITE);

		{
			Rectangle dest = {320, 320, 320, 320};
			Rectangle sour = anim_get_frame(&resources->source[RES_TOP_WALL], 0, 0);
			DrawTexturePro(resources->atlas, sour, dest, Vector2Zero(), 0, WHITE);
		}
		{
			Rectangle dest = {640, 320, 320, 320};
			Rectangle sour = anim_get_frame(&resources->source[RES_TOP_FLOOR], 0, 0);
			DrawTexturePro(resources->atlas, sour, dest, Vector2Zero(), 0, WHITE);
		}
}

void test2_map_generation(res_t *resources, map_t *m)
{
		ClearBackground(DARKBLUE);

		float cw = 1. * GetScreenWidth() / m->width;
		float ch = 1. * GetScreenHeight() / m->height;

		Rectangle dest = {0, 0, ch,ch};

		for(int i = 0; i < m->height; i++)
		{
			for(int j = 0; j < m->width; j++)
			{
				tile_t t = m->tiles[m->width * i + j];

				Color col = BLACK;

				switch(t.type)
				{
					case TILE_WALL: col = BROWN; break;
					case TILE_FLOOR: col = GOLD; break;
					case TILE_ROOM_FLOOR: col = WHITE; break;
					case TILE_ROOM_WALL: col = BLUE; break;
					case TILE_DOOR: col = PINK; break;
					default:
						// do nothin
				}

				DrawRectangleRec(dest, col);
				DrawRectangleLinesEx(dest, 1, GRAY);

				dest.x += ch;
			}

			dest.x = 0;
			dest.y += ch;
		}
}

static void bsp_draw(bsp_t *b, map_t *m, int depth)
{
	if(!b)
		return;

	Rectangle r = b->rect;

	float cw = 1. * GetScreenWidth() / m->width;
	float ch = 1. * GetScreenHeight() / m->height;

	r.x = b->rect.x * ch;
	r.y = b->rect.y * ch;
	r.width = b->rect.width * ch;
	r.height = b->rect.height * ch;

	DrawRectangleLinesEx(r, 1, RED);

	bsp_draw(b->left, m, depth + 1);
	bsp_draw(b->right, m, depth + 1);
}

void test2_map_bsp(map_t *m)
{
	bsp_t *b = m->bsp;
	bsp_draw(b, m, 0);

	float ch = 1. * GetScreenHeight() / m->height;

	for(int i = 0; i < m->rooms_count; i++)
	{
		Rectangle r = m->rooms[i];

		r.x *= ch;
		r.y *= ch;
		r.width *= ch;
		r.height *= ch;

		DrawRectangleLinesEx(r, 1, SKYBLUE);
	}
}
