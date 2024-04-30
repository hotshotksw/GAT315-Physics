#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			CreateBody();
			bodies->position = position;
			bodies->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		// update / draw bodies
		Body* body = bodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// update body position
			body->position = Vector2Add(body->position, body->velocity);
			// draw body
			DrawCircle((int)body->position.x, (int)body->position.y, 10, RED);

			body = body->next; // get next body
		}

		EndDrawing();
	}
	CloseWindow();
	free(bodies);

	return 0;
}

// update bodies
/*for (int i = 0; i < bodyCount; i++)
{
	bodies[i].position = Vector2Add(bodies[i].position, bodies[i].velocity);
	DrawCircle((int)bodies[i].position.x, (int)bodies[i].position.y, 10, RED);
}*/