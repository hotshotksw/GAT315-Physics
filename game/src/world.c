#include "world.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

kwBody* kwBodies = NULL;
int kwBodyCount = 0;
Vector2 kwGravity;

kwBody* CreateBody(Vector2 position, float mass, kwBodyType bodyType)
{
	kwBody* body = (kwBody*)malloc(sizeof(kwBody));
	assert(body);


	memset(body, 0, sizeof(kwBody));
	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
	body->type = bodyType;
	
	//Return new Body
	return body;
}

void AddBody(kwBody* body) 
{
	assert(body);

	// add element to linked list
	body->prev = NULL;
	body->next = kwBodies;

	if (kwBodyCount != NULL)
	{
		kwBodies->prev = body;
	}
	//Update head of elements to new element
	kwBodies = body;
	//Increment body count
	kwBodyCount++;
}

void DestroyBody(kwBody* body)
{
	//Assert if provided Body is not NULL
	assert(body);
	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (body->prev) body->prev->next = body->next;
	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (body->next) body->next->prev = body->prev;
	//If body is the head, update head to 'body->next'
	if (kwBodies == body)
	{
		kwBodies = body->next;
	}
	//Decrement body count
	kwBodyCount--;
	//Free the body
	free(body);
}

void DestroyAllBodies() {
	if (kwBodies == NULL) return;

	for (kwBody* body = kwBodies; body; body->next) {
		DestroyBody(body);
	}
}