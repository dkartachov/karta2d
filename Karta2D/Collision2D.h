#pragma once
#include "SimpleECS/EntityManager.h"
#include "SimpleECS/Components.h"

class Collision2D {
public:
	static bool AABB(Entity& ent1, Entity& ent2) {
		BoxCollider2D ent1BoxCol = *ent1.GetComponent<BoxCollider2D>();
		BoxCollider2D ent2BoxCol = *ent2.GetComponent<BoxCollider2D>();

		if (ent1.GetComponent<Transform2D>()->getPosition().x + 0.5 * ent1BoxCol.getSize().x > ent2.GetComponent<Transform2D>()->getPosition().x - 0.5 * ent2BoxCol.getSize().x &&
			ent1.GetComponent<Transform2D>()->getPosition().x - 0.5 * ent1BoxCol.getSize().x < ent2.GetComponent<Transform2D>()->getPosition().x + 0.5 * ent2BoxCol.getSize().x &&
			ent1.GetComponent<Transform2D>()->getPosition().y + 0.5 * ent1BoxCol.getSize().y > ent2.GetComponent<Transform2D>()->getPosition().y - 0.5 * ent2BoxCol.getSize().y &&
			ent1.GetComponent<Transform2D>()->getPosition().y - 0.5 * ent1BoxCol.getSize().y < ent2.GetComponent<Transform2D>()->getPosition().y + 0.5 * ent2BoxCol.getSize().y) {

			return true;
		}

		return false;
	}

	static void resolveAABBCollisions() {
		for (auto& thisEntity : EntityManager::getInstance().getEntities()) {
			for (auto& entity : EntityManager::getInstance().getEntities()) {
				if (thisEntity->getId() != entity->getId()) {
					if (AABB(*thisEntity, *entity)) {
						std::printf("Collision detected between entities %d and %d\n", thisEntity->getId(), entity->getId());
					}
				}
			}
		}
	}
};