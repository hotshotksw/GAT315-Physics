#pragma once

typedef struct kwSpring
{
	struct kwBody* body1;
	struct kwBody* body2;
	float restLength;
	float k; // stiffness

	struct kwSpring* next;
	struct kwSpring* prev;
} kwSpring_t;

extern kwSpring_t* kwSprings;

kwSpring_t* CreateSpring(struct kwBody* body1, struct kwBody* body2, float restLength, float k);
void AddSpring(kwSpring_t* spring);
void DestroySpring(kwSpring_t* spring);
void DestroyAllSprings();

void ApplySpringForce(kwSpring_t* springs);