#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"
#include "integrator.h"

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
			kwBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(1, 5);
		}

		// apply force
		kwBody* body = kwBodies;
		while (body)
		{
			ApplyForce(body, CreateVector2(0, -50));
			body = body->next;
		}

		// update bodies
		body = kwBodies;
		while (body)
		{
			// update body position
			ExplicitEuler(body, dt);
			ClearForce(body);
			body = body->next;
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		// draw bodies
		body = kwBodies;
		while (body)
		{
			//DrawCircle((int)body->position.x, (int)body->position.y, 10, RED);
			DrawCircleLines((int)body->position.x, (int)body->position.y, body->mass, RED);
			body = body->next; // get next body
		}

		EndDrawing();
	}
	CloseWindow();
	free(kwBodies);

	return 0;
}
