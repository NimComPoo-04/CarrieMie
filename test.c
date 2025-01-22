#include "test.h"

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

void test2_map_rect(map_t *m)
{
	float ch = 1. * GetScreenHeight() / m->height;
	for(int i = 0; i < m->rooms_count; i++)
	{
		Rectangle r = m->rooms[i];
		r.x *= ch; r.width *= ch;
		r.y *= ch; r.height *= ch;

		if(i == 0)
			DrawRectangleRec(r, RED);
		else if(i == 1)
			DrawRectangleRec(r, GREEN);
		else if(i < m->rooms_count * 2/3)
			DrawRectangleRec(r, YELLOW);
		else
			DrawRectangleRec(r, BLUE);
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

void test3_player_and_test(player_t *p, map_t *m, res_t *resources)
{
	float ch = 1. * GetScreenHeight() / m->height;

	test2_map_generation(resources, m);
	Vector2 k = Vector2Scale(p->pos, ch);

	Vector2 vel = {0};
	vel.x = cos(p->look) * p->vel.x * p->size * ch;
	vel.y = sin(p->look) * p->vel.y * p->size * ch;

	player_update(p, m);
	DrawCircleV(k, p->size * ch, SKYBLUE);
	DrawLineV(k, Vector2Add(k, vel), MAGENTA);
	render_player_view(p, m);
}

void test4_player2d_and_3d(player_t *p, map_t *m, res_t *resources)
{
	ClearBackground(DARKBLUE);

	float ch = 1. * GetScreenWidth() / m->width / 2;

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

	Vector2 k = Vector2Scale(p->pos, ch);
	player_update(p, m);
	DrawCircleV(k, p->size * ch, SKYBLUE);

	int width = GetScreenWidth()/2;
	float l = p->look > PI ? p->look - 2 * PI : p->look;

	int wid = 1;

	ray_hit_t rayhits[width / wid];
	int rayhitcounts = 0;

	for(int i = 0; i < width; i += wid)
	{
		float angle = atan2(i - width/2, width / 2) + l;

		ray_hit_t r = ray_project(m, p->pos, angle);

		Vector2 v1 = Vector2Scale(p->pos, ch);
		Vector2 v2 = Vector2Scale(r.hitpos, ch);

		DrawLineV(v1, v2, RED);

		rayhits[rayhitcounts++] = r;
	}


	// Draw 3d now
	int height = GetScreenHeight();
	
	// Drawing floor and celing
	DrawRectangle(width, 0, width, height/2, SKYBLUE);
	DrawRectangle(width, height/2, width, height/2, GREEN);

	float wall_height = height;

	Rectangle wall = anim_get_frame(&resources->source[RES_TOP_WALL], 0, 0);
	Rectangle room_wall = anim_get_frame(&resources->source[RES_TOP_FLOOR], 0, 0);

	for(int i = 0; i < rayhitcounts; i++)
	{
		ray_hit_t r = rayhits[i];

		float hei = wall_height / (r.dist * cos(r.angle - p->look));

		Rectangle rec = {i * wid + width, height / 2 - hei / 2, wid, hei};

		Color col = {0, 0, 0, 0};
		Rectangle src = {0};

		switch(r.hittile.type)
		{
			case TILE_WALL:      src = wall; col = BROWN; break;
			case TILE_ROOM_WALL: src = room_wall; col = BLUE; break;
			default: // do nothin
		}

		src.x += src.width * r.intrp - 0.5;
		src.width = 1;

		DrawTexturePro(resources->atlas, src, rec, Vector2Zero(), 0, col);	
//		DrawRectangleRec(rec, ColorLerp(col, BLACK, r.intrp));
	}
}

void test4_player_3d(player_t *p, map_t *m, res_t *resources)
{
	// First get a list of all the quads that do require rendering, for now thats all i presume lol

	Camera3D cam = {
		{p->pos.x, 0, p->pos.y},
		{
			p->pos.x + cos(p->look),
			0,
			p->pos.y + sin(p->look)
		},
		{0, 1, 0},
		45,
		CAMERA_PERSPECTIVE
	};

	BeginMode3D(cam);

	ClearBackground(BLACK);

	for(int i = 0; i < m->height; i++)
	{
		for(int j = 0; j < m->width; j++)
		{
			tile_t t = m->tiles[m->width * i + j];

			Color col = BLACK;

			Vector3 pos = { j + 0.5, 0, i + 0.5};
			Vector3 pln = { j + 0.5, -0.5, i + 0.5};

			switch(t.type)
			{
				case TILE_FLOOR:      DrawPlane(pln, (Vector2){1, 1}, GOLD); break;
				case TILE_ROOM_FLOOR: DrawPlane(pln, (Vector2){1, 1}, WHITE); break;

				case TILE_WALL:      DrawCube(pos, 1, 1, 1, BROWN); break;
				case TILE_ROOM_WALL: DrawCube(pos, 1, 1, 1, BLUE); break;
				case TILE_DOOR:      DrawCube(pos, 1, 1, 1, PINK); break;

				default:
						      // do nothin
			}
		}
	}

	EndMode3D();

	player_update(p, m);
}

void test5_pathfinder(player_t *p, path_t *path, res_t *resources)
{
	map_t *m = path->map;

	int sx = 1;
	int sy = 1;
	int ex = m->width - 2;
	int ey = m->height - 2;

	path_compute(path, sx, sy, ex, ey);

	test2_map_generation(resources, m);

	float cw = 1. * GetScreenWidth() / m->width;
	float ch = 1. * GetScreenHeight() / m->height;

	Rectangle dest = {0, 0, ch,ch};

	for(int i = 0; i < m->height; i++)
	{
		for(int j = 0; j < m->width; j++)
		{
			int t = path->cells[m->width * i + j].py;
			if(t >= 0)
			{
				Color col = DARKGRAY;

				DrawRectangleRec(dest, col);
				DrawRectangleLinesEx(dest, 1, GRAY);
			}

			dest.x += ch;
		}

		dest.x = 0;
		dest.y += ch;
	}

	while((ex != sx || ey != sy) && (ex >= 0))
	{
		Rectangle rec = {ex * ch, ey * ch, ch, ch};
		DrawRectangleRec(rec, PURPLE);

		int nx = path->cells[m->width * ey + ex].px;
		int ny = path->cells[m->width * ey + ex].py;

		ex = nx;
		ey = ny;
	}

	Rectangle rec = {ex * ch, ey * ch, ch, ch};
	DrawRectangleRec(rec, PURPLE);
}
