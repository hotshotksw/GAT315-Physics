#pragma once
#include "body.h"

void ExplicitEuler(kwBody* body, float timestep)
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(Vector2Scale(body->force, 1 / body->mass), timestep));
}