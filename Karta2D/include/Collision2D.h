#pragma once
#include "EntityManager.h"
#include "Components.h"

class Collision2D {
public:
	// collision detection
	static bool AABB(std::pair<Vector2D, Vector2D>& squareA, std::pair<Vector2D, Vector2D>& squareB);
	static bool BoxBox(Entity& boxA, Entity& boxB);
	static bool CircleCircle(Entity& thisEntity, Entity& entity);
	static bool BoxCircle(Entity& box, Entity& circle);
	static bool overlap(const Vector2D normal, const std::vector<Vector2D>& entAVertices, const std::vector<Vector2D>& entBVertices);
	static bool satDetection(Entity& entA, Entity& entB);

	// normals
	static float getOverlap(const Vector2D normal, const std::vector<Vector2D> entAVertices, const std::vector<Vector2D> entBVertices);
	static std::pair<Vector2D, float> getSATNormal(Entity& entA, Entity& entB);
	static std::pair<Vector2D, float> getCircleCircleNormal(Entity& entA, Entity& entB);
	static std::pair<Vector2D, float> getBoxCircleNormal(Entity& box, Entity& circle);

	// collision resolution
	static void resolveCollision(Entity& thisEntity, Entity& entity, Vector2D collisionNormal, float penetration);
	static void resolveCollisions();
};