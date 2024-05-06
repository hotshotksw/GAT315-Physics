#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"
#include "integrator.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000


void RenderGUI(float fps, float dt, int mode, int rotation, int windowWidth, int windowHeight);
void EmissionOne(Vector2 position);
void EmissionTwo(Vector2 position);
void EmissionThree(Vector2 position, int rotation);
void EmissionFour(Vector2 position);

int main(void)
{
	int windowWidth = 1280;
	int windowHeight = 720;
	
	InitWindow(windowWidth, windowHeight, "Physics Engine");
	SetTargetFPS(60);

	int mode = 1;
	int rotation = 0;
	float timer = 0;

	// initialize world
	kwGravity = (Vector2){ 0, 30 };

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		
		switch (mode)
		{
		case 1:
			if (IsMouseButtonDown(0))
			{
				EmissionOne(position);
			}
			break;
		case 2:
			if (IsMouseButtonPressed(0))
			{
				EmissionTwo(position);
			}
			break;
		case 3:
			if (IsKeyDown(KEY_UP))
			{
				rotation++;
			} else if (IsKeyDown(KEY_DOWN))
			{
				rotation--;
			} else if (IsKeyPressed(KEY_EQUAL))
			{
				rotation++;
			}
			else if (IsKeyPressed(KEY_MINUS))
			{
				rotation--;
			}
			if (rotation > 360)
			{
				rotation -= 360;
			}
			else if (rotation < 0)
			{
				rotation += 360;
			}

			if (IsMouseButtonDown(0))
			{
				EmissionThree(position, rotation);
			}
			break;
		case 4:
			if (IsMouseButtonDown(0))
			{
				EmissionOne(position);
			}
		}

		if (IsKeyPressed(258))
		{
			mode++;
			if (mode > 4) mode = 1;
		}

		// apply force
		kwBody* body = kwBodies;
		while (body)
		{
			//ApplyForce(body, CreateVector2(0, -50), FM_FORCE);
			body = body->next;
		}

		// update bodies
		for (kwBody* body = kwBodies; body; body = body->next)
		{
			Step(body, dt);
		}

		body = kwBodies;
		while (body)
		{
			// update body position
			Step(body, dt);
			body = body->next;
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		// stats
		RenderGUI(fps, dt, mode, rotation, windowWidth, windowHeight);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		// draw bodies
		body = kwBodies;
		while (body)
		{
			//DrawCircle((int)body->position.x, (int)body->position.y, 10, RED);
			switch (mode)
			{
			case 1:
				DrawCircleLines((int)body->position.x, (int)body->position.y, body->mass, RED);
				break;
			case 2:
				DrawRectangle((int)body->position.x, (int)body->position.y, 50, 50, RAYWHITE);
				break;
			case 3:
				DrawCircleLines((int)body->position.x, (int)body->position.y, body->mass * 2, LIME);
				break;
			case 4:
				timer += dt;
				if (timer > 64)
				{
					DrawCircle((int)body->position.x, (int)body->position.y, body->mass * 2, (Color) { (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255)
					});
				}
				if (timer > 256)
				{
					timer = 0;
				}
				break;
			}
		
			body = body->next; // get next body
		}

		EndDrawing();
	}
	CloseWindow();
	free(kwBodies);

	return 0;
}

void RenderGUI(float fps, float dt, int mode, int rotation, int windowWidth, int windowHeight) {
	DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
	DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);
	DrawText(TextFormat("EMISSION MODE: %i", mode), (int)(windowWidth / 2) - 99, 11, 20, SKYBLUE);
	DrawText(TextFormat("EMISSION MODE: %i", mode), (int)(windowWidth / 2) - 100, 10, 20, BLUE);

	if (mode == 3)
	{
		DrawText(TextFormat("ROTATION: %i", rotation), (int)(windowWidth) - 175, 10, 20, BEIGE);
	}
}

void EmissionOne(Vector2 position) {
	kwBody* body = CreateBody();
	body->position = position;
	body->mass = GetRandomFloatValue(1, 5);
	body->inverseMass = 1 / body->mass;
	body->type = BT_DYNAMIC;
	body->damping = 2.5f;
	body->gravityScale = 20;
	ApplyForce(body, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100) }, FM_VELOCITY);
}

/*
*  # # # #
* # # # # 
*  # # # #
* # # # # 
*  # # # #
* # # # # 
*  # # # #
* # # # # 
*/

void EmissionTwo(Vector2 position) {
	Vector2 eBodies[] = { 
		{ -3, -4 }, { -1, -4 }, { 1, -4 }, { 3, -4 },
		{ -4, -3 }, { -2, -3 }, { 0, -3 }, { 2, -3 }
	};
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			kwBody* body = CreateBody();
			body->position = position;
			body->mass = 5;
			body->inverseMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.5f;
			body->gravityScale = 10;
			ApplyForce(body, (Vector2) { eBodies[j].x * 120, (eBodies[j].y + (i*2)) * 120 }, FM_VELOCITY);
		}
	}
	
}

void EmissionThree(Vector2 position, int rotation) {
	kwBody* body = CreateBody();
	body->position = position;
	body->mass = GetRandomFloatValue(1, 5);
	body->inverseMass = 1 / body->mass;
	body->type = BT_DYNAMIC;
	body->damping = 2.5f;
	body->gravityScale = 20;
	ApplyForce(body, (Vector2) { 500.0f * (float)(-sin(rotation)), 500.0f * (float)(cos(rotation)) }, FM_VELOCITY);
}

void EmissionFour(Vector2 position) {
	kwBody* body = CreateBody();
	body->position = position;
	body->mass = GetRandomFloatValue(1, 5);
	body->inverseMass = 1 / body->mass;
	body->type = BT_DYNAMIC;
	body->damping = 2.5f;
	body->gravityScale = 20;
	ApplyForce(body, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100) }, FM_VELOCITY);
}