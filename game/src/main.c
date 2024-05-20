#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

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
	kwBody* selectedBody = NULL;
	kwBody* connectBody = NULL;
	ncContact_t* contacts = NULL;

	int windowWidth = 1280;
	int windowHeight = 720;

	float fixedTimestep = 1.0f / 60;
	float timeAccumulator = 0;
	
	InitWindow(windowWidth, windowHeight, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	int mode = 1;
	int rotation = 0;
	float timer = 0;

	// initialize world
	kwGravity = (Vector2){ 0, -1 };

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * -0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(kwBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		switch (mode)
		{
		case 1:
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)))
			{
				EmissionOne(position);
			}
			break;
		case 2:
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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

			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
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

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				kwSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), kwEditorData.StiffnessValue);
				AddSpring(spring);
			}
		}

		if (IsKeyPressed(258))
		{
			mode++;
			if (mode > 4) mode = 1;
		}

		timeAccumulator += dt;
		while (timeAccumulator >= fixedTimestep) {
			timeAccumulator -= fixedTimestep;
			// apply force
			ApplyGravitation(kwBodies, kwEditorData.GravitationValue);
			ApplySpringForce(kwSprings);

			// update bodies
			for (kwBody* body = kwBodies; body; body = body->next)
			{
				Step(body, dt);
			}

			// collision
			contacts = NULL;
			CreateContacts(kwBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);

			// render
			BeginDrawing();
			ClearBackground(BLACK);

			// stats
			RenderGUI(fps, dt, mode, rotation, windowWidth, windowHeight);

			//DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

			// Draw Springs
			for (kwSpring_t* spring = kwSprings; spring; spring = spring->next)
			{
				Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
				Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
				DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
			}
			// draw bodies
			for (kwBody* body = kwBodies; body; body = body->next)
			{
				Vector2 screen = ConvertWorldToScreen(body->position);
				switch (mode)
				{
				case 1:
					DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
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
						DrawCircle((int)body->position.x, (int)body->position.y, body->mass * 2, (Color) {
							(int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255)
						});
					}
					if (timer > 256)
					{
						timer = 0;
					}
					break;
				}
			}
			// Draw Contacts
			for (ncContact_t* contact = contacts; contact; contact = contact->next)
			{
				Vector2 screen = ConvertWorldToScreen(contact->body1->position);
				DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
			}
		}
		DrawEditor(position);

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
	kwBody* body = CreateBody(ConvertScreenToWorld(position), kwEditorData.MassMinValue, kwEditorData.BodyTypeActive);
	body->damping = kwEditorData.DampingValue;
	body->gravityScale = kwEditorData.BodyGravityValue;
	body->color = WHITE;//(Color){ (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255), (int)GetRandomFloatValue(0, 255), 255 };
	body->restitution = kwEditorData.RestitutionValue * 5;
	AddBody(body);
}

void EmissionTwo(Vector2 position) {
	/*Vector2 eBodies[] = { 
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
	}*/
	
}

void EmissionThree(Vector2 position, int rotation) {
	/*kwBody* body = CreateBody();
	body->position = position;
	body->mass = GetRandomFloatValue(1, 5);
	body->inverseMass = 1 / body->mass;
	body->type = BT_DYNAMIC;
	body->damping = 2.5f;
	body->gravityScale = 20;
	ApplyForce(body, (Vector2) { 500.0f * (float)(-sin(rotation)), 500.0f * (float)(cos(rotation)) }, FM_VELOCITY);*/
}

void EmissionFour(Vector2 position) {
	/*kwBody* body = CreateBody();
	body->position = position;
	body->mass = GetRandomFloatValue(1, 5);
	body->inverseMass = 1 / body->mass;
	body->type = BT_DYNAMIC;
	body->damping = 2.5f;
	body->gravityScale = 20;
	ApplyForce(body, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100) }, FM_VELOCITY);*/
}