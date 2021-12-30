#pragma once
#include "SimpleECS/EntityManager.h"
#include "SimpleECS/Components.h"

class Collision2D {
public:
	static bool AABB(Entity& thisEntity, Entity& entity) {
		Vector2D thisEntityPos = thisEntity.GetComponent<Transform2D>()->getPosition();
		Vector2D thisEntityBox = thisEntity.GetComponent<BoxCollider2D>()->getSize();

		Vector2D entityPos = entity.GetComponent<Transform2D>()->getPosition();
		Vector2D entityBox = entity.GetComponent<BoxCollider2D>()->getSize();

		if (thisEntityPos.x + 0.5 * thisEntityBox.x > entityPos.x - 0.5 * entityBox.x &&
			thisEntityPos.x - 0.5 * thisEntityBox.x < entityPos.x + 0.5 * entityBox.x &&
			thisEntityPos.y + 0.5 * thisEntityBox.y > entityPos.y - 0.5 * entityBox.y &&
			thisEntityPos.y - 0.5 * thisEntityBox.y < entityPos.y + 0.5 * entityBox.y) {

			return true;
		}

		return false;
	}

	static Vector2D getNormal(Entity& thisEntity, Entity& entity) {
		Vector2D thisEntityPos = thisEntity.GetComponent<Transform2D>()->getPosition();
		Vector2D thisEntityBox = thisEntity.GetComponent<BoxCollider2D>()->getSize();

		Vector2D entityPos = entity.GetComponent<Transform2D>()->getPosition();
		Vector2D entityBox = entity.GetComponent<BoxCollider2D>()->getSize();

		double dx = 0;
		if (entityPos.x > thisEntityPos.x) {
			dx = fabs(((double)thisEntityPos.x - (double)entityPos.x) + 0.5 * ((double)thisEntityBox.x + (double)entityBox.x));
		}
		else {
			dx = fabs(((double)thisEntityPos.x - (double)entityPos.x) - 0.5 * ((double)thisEntityBox.x + (double)entityBox.x));
		}

		double dy = 0;
		if (entityPos.y > thisEntityPos.y) {
			dy = fabs(((double)thisEntityPos.y - (double)entityPos.y) + 0.5 * ((double)thisEntityBox.y + (double)entityBox.y));
		}
		else {
			dy = fabs(((double)thisEntityPos.y - (double)entityPos.y) - 0.5 * ((double)thisEntityBox.y + (double)entityBox.y));
		}

		if (dx <= dy) {
			if (entityPos.x > thisEntityPos.x) {
				return Vector2D(-1, 0);
			}
			else {
				return Vector2D(1, 0);
			}
		}
		else {
			if (entityPos.y > thisEntityPos.y) {
				return Vector2D(0, -1);
			}
			else {
				return Vector2D(0, 1);
			}
		}
	}

	static void resolveAABBCollisions() {
		std::vector<Entity*>& entities = EntityManager::getInstance().getEntities();

		for (auto& thisEntity : entities) {
			for (auto& entity : entities) {
				if (thisEntity->getId() != entity->getId()) {
					if (AABB(*thisEntity, *entity)) {
						Vector2D n = getNormal(*thisEntity, *entity);
						std::printf("Normal for Entity '%s' is (%d, %d)\n", thisEntity->getName().c_str(), (int)n.x, (int)n.y);
						//std::printf("Collision detected between entities %d and %d\n", thisEntity->getId(), entity->getId());
					}
				}
			}
		}
		std::printf("\n");
	}
};