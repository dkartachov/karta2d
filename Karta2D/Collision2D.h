#pragma once
#include "SimpleECS/EntityManager.h"
#include "SimpleECS/Components.h"
#include <tuple>

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

	static void resolveCollision(Entity& thisEntity, Entity& entity, Vector2D collisionNormal) {
		Vector2D thisEntityVel = thisEntity.GetComponent<Rigidbody2D>()->getVelocity();
		Vector2D entityVel = entity.GetComponent<Rigidbody2D>()->getVelocity();

		Vector2D relVelocity = thisEntityVel - entityVel;
		float velAlongNormal = Vector2D::dot(relVelocity, collisionNormal);

		float restitution = 1;
		float j = -(1 + restitution) * velAlongNormal;
		j /= 1 / thisEntity.GetComponent<Rigidbody2D>()->getMass() + 1 / entity.GetComponent<Rigidbody2D>()->getMass();

		Vector2D impulse = j * collisionNormal;

		thisEntity.GetComponent<Rigidbody2D>()->setVelocity(thisEntityVel + (1 / thisEntity.GetComponent<Rigidbody2D>()->getMass()) * impulse);
		entity.GetComponent<Rigidbody2D>()->setVelocity(entityVel - (1 / entity.GetComponent<Rigidbody2D>()->getMass()) * impulse);
	}

	static void resolveCollisions(std::vector<std::tuple<Entity*, Entity*, Vector2D>>& collisionTuples) {
		for (auto& colTuple : collisionTuples) {
			Entity* entA = std::get<0>(colTuple);
			Entity* entB = std::get<1>(colTuple);
			Vector2D collisionNormal = std::get<2>(colTuple);

			resolveCollision(*entA, *entB, collisionNormal);
		}
	}

	static void resolveAABBCollisions() {
		std::vector<Entity*>& entities = EntityManager::getInstance().getEntities();
		std::vector<std::tuple<Entity*, Entity*, Vector2D>> collisionTuples;

		for (auto& thisEntity : entities) {
			for (auto& entity : entities) {
				if (thisEntity->getId() != entity->getId()) {
					if (AABB(*thisEntity, *entity)) {
						Vector2D collisionNormal = getNormal(*thisEntity, *entity);

						auto it = std::find_if(collisionTuples.begin(), collisionTuples.end(), [&](std::tuple<Entity*, Entity*, Vector2D> colTuple) {
							Entity* entA = std::get<0>(colTuple);
							Entity* entB = std::get<1>(colTuple);

							bool foundFirst = entA->getId() == thisEntity->getId() || entA->getId() == entity->getId();
							bool foundSecond = entB->getId() == thisEntity->getId() || entB->getId() == entity->getId();

							return foundFirst && foundSecond;
						});

						if (it != collisionTuples.end()) {
							continue;
						}

						collisionTuples.push_back({ thisEntity, entity, collisionNormal });

						//std::printf("Normal for Entity '%s' is (%d, %d)\n", thisEntity->getName().c_str(), (int)collisionNormal.x, (int)collisionNormal.y);
						//std::printf("Collision detected between entities %d and %d\n", thisEntity->getId(), entity->getId());
					}
				}
			}
		}

		resolveCollisions(collisionTuples);

		std::printf("\n");
	}
};