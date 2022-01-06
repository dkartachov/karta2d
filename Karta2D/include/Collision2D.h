#pragma once
#include "EntityManager.h"
#include "Components.h"
#include <tuple>

class Collision2D {
public:
	// collision detection
	static bool AABB(Entity& thisEntity, Entity& entity);
	static bool CircleCircle(Entity& thisEntity, Entity& entity);

	// normals
	static std::pair<Vector2D, float> getAABBNormal(Entity& thisEntity, Entity& entity);
	static std::pair<Vector2D, float> getCircleCircleNormal(Entity& entA, Entity& entB);

	// collision resolution
	static void resolveCollision(Entity& thisEntity, Entity& entity, Vector2D collisionNormal, float penetration);
	static void resolveCollisionTuples(std::vector<std::tuple<Entity*, Entity*, Vector2D, float>>& collisionTuples);
	static void resolveCollisions();
};