#include "spring.h"
#include "body.h"
//#include "force.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

kwSpring_t* kwSprings = NULL;

kwSpring_t* CreateSpring(struct kwBody* body1, struct kwBody* body2, float restLength, float k)
{
	kwSpring_t* spring = (kwSpring_t*)malloc(sizeof(kwSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(kwSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	//Return new Body
	return spring;
}


void AddSpring(kwSpring_t* spring)
{
	assert(spring);

	spring->prev = NULL;
	spring->next = kwSprings;

	if (kwSprings)
	{
		kwSprings->prev = spring;
	}
	kwSprings = spring;
}

void DestroySpring(kwSpring_t* spring)
{
	assert(spring);
	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;
	if (kwSprings == spring)
	{
		kwSprings = spring->next;
	}
	free(spring);
}

void ApplySpringForce(kwSpring_t* springs)
{
	for (kwSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body2->position, spring->body1->position); //<get direction vector from body2 to body1>
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength; //<compute displacement from current length to resting length>;
		float force = spring->k * x; // f = -kx <- Hooke's Law

		Vector2 ndirection = Vector2Normalize(direction); //<get direction normal>

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE);
	}
}