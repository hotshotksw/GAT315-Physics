#include "world.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

kwBody* kwBodies = NULL;
int kwBodyCount = 0;
Vector2 kwGravity;

kwBody* CreateBody()
{
	kwBody* body = (kwBody*)malloc(sizeof(kwBody));
	assert(body);

	memset(body, 0, sizeof(kwBody));

	// add body to linked list
	body->prev = NULL;
	body->next = kwBodies;
	if (kwBodyCount > 0)
	{
		kwBodies->prev = body->prev;
	}
	//Update head of the list to new Body
	kwBodies = body;
	//Increment body count
	kwBodyCount++;
	//Return new Body
	return body;
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