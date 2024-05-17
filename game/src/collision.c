#include "collision.h"
#include "contact.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Function to determine if two bodies intersect.
bool Intersects(kwBody* body1, kwBody* body2)
{
    // Calculate distance between body positions.
    float distance = Vector2Distance(body1->position, body2->position);
    // Calculate the sum of radii (masses).
    float radius = body1->mass + body2->mass;
    // Return true if distance is less than or equal to radius.
    return distance <= radius;
}

// Function to create contacts between bodies.
void CreateContacts(kwBody* bodies, ncContact_t** contacts)
{
    // Loop through each pair of bodies.
    for (kwBody* body1 = bodies; body1; body1 = body1->next)
    {
        for (kwBody* body2 = bodies; body2; body2 = body2->next)
        {
            // Skip if both bodies are the same.
            if (body1 == body2) continue;
            // Skip if both bodies are not dynamic.
            if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

            // Check if bodies intersect.
            if (Intersects(body1, body2))
            {
                // Generate contact and add to contact list.
                ncContact_t* contact = GenerateContact(body1, body2);
                AddContact(contact, contacts);
            }
        }
    }
}

// Function to generate contact information between two bodies.
ncContact_t* GenerateContact(kwBody* body1, kwBody* body2)
{
    // Allocate memory for contact structure.
    ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
    assert(contact); // Ensure memory allocation was successful.

    // Initialize contact structure.
    memset(contact, 0, sizeof(ncContact_t));
    contact->body1 = body1;
    contact->body2 = body2;

    // Calculate direction between bodies.
    Vector2 direction = Vector2Subtract(body1->position, body2->position);
    float distance = Vector2Length(direction);

    // If bodies are exactly overlapping, introduce random direction.
    if (distance == 0)
    {
        direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
    }

    // Calculate depth of penetration, normal direction, and restitution.
    float radius = (body1->mass + body2->mass);
    contact->depth = radius - distance;
    contact->normal = Vector2Normalize(direction);
    contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

    return contact;
}

// Function to separate overlapping bodies based on contacts.
void SeparateContacts(ncContact_t* contacts)
{
    // Loop through contacts.
    for (ncContact_t* contact = contacts; contact; contact = contact->next)
    {
        // Calculate total inverse mass.
        float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;
        // Calculate separation impulse.
        Vector2 separation = Vector2Scale(contact->normal, contact->depth / totalInverseMass);

        // Move bodies apart based on their inverse masses.
        contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));
        contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(separation, -contact->body2->inverseMass));
    }
}

// Function to resolve collision impulses between bodies.
void ResolveContacts(ncContact_t* contacts)
{
    // Loop through contacts.
    for (ncContact_t* contact = contacts; contact; contact = contact->next)
    {
        // Calculate relative velocity.
        Vector2 rv = Vector2Subtract(contact->body1->position, contact->body2->position);
        float nv = Vector2DotProduct(rv, contact->normal);

        // Skip if objects are moving apart.
        if (nv > 0) continue;

        // Calculate impulse magnitude.
        float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;
        float impulseMagnitude = (-(1 + contact->restitution) * nv / totalInverseMass);

        // Calculate impulse vector.
        Vector2 impulse = Vector2Scale(contact->normal, impulseMagnitude);

        // Apply impulse to bodies.
        ApplyForce(contact->body1, impulse, FM_IMPULSE);
        ApplyForce(contact->body2, Vector2Negate(impulse), FM_IMPULSE);
    }
}