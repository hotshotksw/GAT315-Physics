#include "render.h"
#include "raymath.h"

// Define the screen size, screen zoom level, and view size
Vector2 ncScreenSize = { 1280, 720 };
float ncScreenZoom = 1.0f;
float ncViewSize = 25.0f;

// Convert screen coordinates to world coordinates
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate the ratio of screen width to screen height
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view in world space
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view in world space
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize the screen coordinates to the range [0, 1]
    float nx = screen.x / ncScreenSize.x;
    float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

    // Convert normalized screen coordinates to world coordinates
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world;
}

// Convert world coordinates to screen coordinates
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate the ratio of screen width to screen height
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view in world space
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view in world space
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize the world coordinates to the range [0, 1]
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    // Convert normalized world coordinates to screen coordinates
    Vector2 screen;
    screen.x = nx * ncScreenSize.x;
    screen.y = (1.0f - ny) * ncScreenSize.y;

    return screen;
}

// Convert world value to pixels
float ConvertWorldToPixel(float world)
{
    // Calculate the ratio of screen width to screen height
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view in world space
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Convert world value to pixels based on the screen size and zoom level
    float nx = world / extents.x;
    float pixel = nx * ncScreenSize.x;

    return pixel;
}