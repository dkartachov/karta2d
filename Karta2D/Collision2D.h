#pragma once
#include "SimpleECS/EntityManager.h"
#include "SimpleECS/Components.h"

class Collision2D {
public:
	static bool AABB(Entity& ent1, Entity& ent2) {
		Vector2D ent1Position = ent1.GetComponent<Transform2D>()->getPosition();
		Vector2D ent1Box = ent1.GetComponent<BoxCollider2D>()->getSize();

		Vector2D ent2Position = ent2.GetComponent<Transform2D>()->getPosition();
		Vector2D ent2Box = ent2.GetComponent<BoxCollider2D>()->getSize();

		if (ent1Position.x + 0.5 * ent1Box.x > ent2Position.x - 0.5 * ent2Box.x &&
			ent1Position.x - 0.5 * ent1Box.x < ent2Position.x + 0.5 * ent2Box.x &&
			ent1Position.y + 0.5 * ent1Box.y > ent2Position.y - 0.5 * ent2Box.y &&
			ent1Position.y - 0.5 * ent1Box.y < ent2Position.y + 0.5 * ent2Box.y) {

			return true;
		}

		return false;
	}

	static void resolveAABBCollisions() {
		std::vector<Entity*>& entities = EntityManager::getInstance().getEntities();

		for (auto& thisEntity : entities) {
			for (auto& entity : entities) {
				if (thisEntity->getId() != entity->getId()) {
					if (AABB(*thisEntity, *entity)) {
						std::printf("Collision detected between entities %d and %d\n", thisEntity->getId(), entity->getId());
					}
				}
			}
		}
	}
};