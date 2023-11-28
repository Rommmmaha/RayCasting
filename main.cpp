#include"raylib.h"
#include<vector>
#include<cmath>
#include<ctime>
#include<string>
#define M_PI 3.14159265359
int mode = 0;
struct _wall
{
public:
	Vector2 a, b;
	Color c = WHITE;
	float w = 3;
	void wallDraw() { DrawLineEx(a, b, w, c); }
};
double distanceBetweenPoints(Vector2 p1, Vector2 p2)
{
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return std::sqrt(dx * dx + dy * dy);
}
bool doesCollide(_wall segment, Vector2 dot)
{
	double distanceToStart = distanceBetweenPoints(segment.a, dot);
	double distanceToEnd = distanceBetweenPoints(segment.b, dot);
	double segmentLength = distanceBetweenPoints(segment.a, segment.b);
	if (distanceToStart + distanceToEnd <= segmentLength + segment.w / 10.0) { return true; }
	return false;
}
void simulateRay(Vector2 Mouse, float angle, std::vector<_wall> walls, Vector2 screen)
{
	int end = 0;
	float i;
	Vector2 dot = Mouse, dot2 = { 0,0 };
	angle = angle / 180.0 * M_PI;
	for (i = 0; !end; i += 2)
	{
		dot = { Mouse.x + i * cos(angle),Mouse.y + i * sin(angle) };
		if ((0 > dot.x || dot.x > screen.x) || (0 > dot.y || dot.y > screen.y)) { end = 1; }
		for (auto wall : walls) { if (doesCollide(wall, dot)) { end = 2; } }
	}
	if (mode == 0 || mode == 1) { DrawLineEx(Mouse, dot, 1, YELLOW); }
	if (end == 2)
	{
		if (mode == 2 || mode == 3) { DrawLineEx(Mouse, dot, 1, YELLOW); }
		if (mode == 1 || mode == 3)
		{
			for (; end; i += 10)
			{
				dot2 = { Mouse.x + i * cos(angle),Mouse.y + i * sin(angle) };
				if ((0 > dot2.x || dot2.x > screen.x) || (0 > dot2.y || dot2.y > screen.y)) { end = 0; }
			}
			DrawLineEx(dot, dot2, 2, RED);
		}
	}
}
int main(void)
{
	srand(time(0));
	Vector2 screen = { 500,500 };
	std::vector<_wall> walls;
	_wall tmp_wall;
	std::string text = "";
	bool LEFT_BUTTON = 0;
	int rays = 90;
	float tmp = 0;
	InitWindow(screen.x, screen.y, "raylib - raycast test");
	while (!WindowShouldClose())
	{
		tmp += 1.0 * GetFrameTime();
		if (tmp > 360) { tmp -= 360; }
		Vector2 Mouse = { float(GetMouseX()) , float(GetMouseY()) };
		BeginDrawing();
		{
			ClearBackground(BLACK);
			if (IsKeyPressed(KEY_KP_1)) { mode = (mode + 1) % 4; }
			if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) { walls.clear(); }
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { if (walls.size() > 0) { walls.pop_back(); } }
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				if (!LEFT_BUTTON) { tmp_wall.a = Mouse; tmp_wall.b = Mouse; }
				LEFT_BUTTON = true;
				tmp_wall.b = Mouse;
				tmp_wall.wallDraw();
			}
			else
			{
				if (LEFT_BUTTON)
				{
					if (tmp_wall.a.x != tmp_wall.b.x || tmp_wall.a.y != tmp_wall.b.y) { walls.push_back({ tmp_wall.a,tmp_wall.b,GRAY }); }
					LEFT_BUTTON = false;
				}
				else
				{
					rays += GetMouseWheelMove() * 2;
					if (rays < 0) { rays = 0; }
					for (int i = 0; i < rays; i++) { simulateRay(Mouse, 360.0 / rays * i + tmp, walls, screen); }
				}
			}
			DrawCircleV(Mouse, 10, GRAY);
			for (auto _wall : walls) { _wall.wallDraw(); }
			DrawRectangle(0, 0, 100, 24, BLACK);
			DrawFPS(2, 2);
			//DrawText(text.c_str(), 2, 20, 10, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
