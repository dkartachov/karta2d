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

			//std::printf("Collision detected between '%s' and '%s'\n", thisEntity.getName().c_str(), entity.getName().c_str());
			return true;
		}

		return false;
	}

	static bool CircleCircle(Entity& thisEntity, Entity& entity) {
		Vector2D thisEntityPos = thisEntity.GetComponent<Transform2D>()->getPosition();
		float thisEntityRadius = thisEntity.GetComponent<CircleCollider2D>()->getRadius();

		Vector2D entityPos = entity.GetComponent<Transform2D>()->getPosition();
		float entityRadius = entity.GetComponent<CircleCollider2D>()->getRadius();

		Vector2D relPos = thisEntityPos - entityPos;
		float radii = thisEntityRadius + entityRadius;

		if (relPos.MagnitudeSquared() < radii * radii) {
			//std::printf("Collision detected between '%s' and '%s'\n", thisEntity.getName().c_str(), entity.getName().c_str());
			return true;
		}

		return false;
	}

	static std::pair<Vector2D, float> getAABBNormal(Entity& thisEntity, Entity& entity) {
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
				return { Vector2D(-1, 0), dx };
			}
			else {
				return { Vector2D(1, 0), dx };
			}
		}
		else {
			if (entityPos.y > thisEntityPos.y) {
				return { Vector2D(0, -1), dy };
			}
			else {
				return { Vector2D(0, 1), dy };
			}
		}
	}

	static std::pair<Vector2D, float> getCircleCircleNormal(Entity& entA, Entity& entB) {
		Vector2D normal = (entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition()).normalize();
		float penetration = fabs((entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition()).Magnitude() - (entA.GetComponent<CircleCollider2D>()->getRadius() + entB.GetComponent<CircleCollider2D>()->getRadius()));
		return { normal, penetration };
	}

	static void resolveCollision(Entity& thisEntity, Entity& entity, Vector2D collisionNormal, float penetration) {
		bool thisEntityHasRb = thisEntity.HasComponent<Rigidbody2D>();
		bool entityHasRb = entity.HasComponent<Rigidbody2D>();

		Vector2D thisEntityVel = thisEntityHasRb ? thisEntity.GetComponent<Rigidbody2D>()->getVelocity() : zeroVector;
		Vector2D entityVel = entityHasRb ? entity.GetComponent<Rigidbody2D>()->getVelocity() : zeroVector;
		
		Vector2D relVelocity = thisEntityVel - entityVel;
		float velAlongNormal = Vector2D::dot(relVelocity, collisionNormal);

		float restitution = 0.5;
		float j = -(1 + restitution) * velAlongNormal;

		float thisEntityInvMass = thisEntityHasRb ? 1 / thisEntity.GetComponent<Rigidbody2D>()->getMass() : 0;
		float entityInvMass = entityHasRb ? 1 / entity.GetComponent<Rigidbody2D>()->getMass() : 0;

		j /= thisEntityInvMass + entityInvMass;

		Vector2D impulse = j * collisionNormal;

		if (thisEntityHasRb && entityHasRb) {
			thisEntity.GetComponent<Transform2D>()->translate(0.5 * penetration * collisionNormal);
			thisEntity.GetComponent<Rigidbody2D>()->setVelocity(thisEntityVel + thisEntityInvMass * impulse);

			entity.GetComponent<Transform2D>()->translate(-0.5 * penetration * collisionNormal);
			entity.GetComponent<Rigidbody2D>()->setVelocity(entityVel - entityInvMass * impulse);

			return;
		}

		if (thisEntityHasRb) {
			thisEntity.GetComponent<Transform2D>()->translate(penetration * collisionNormal);
			thisEntity.GetComponent<Rigidbody2D>()->setVelocity(thisEntityVel + thisEntityInvMass * impulse);
		}

		if (entityHasRb) {
			entity.GetComponent<Transform2D>()->translate(-penetration * collisionNormal);
			entity.GetComponent<Rigidbody2D>()->setVelocity(entityVel - entityInvMass * impulse);
		}
	}

	static void resolveCollisionTuples(std::vector<std::tuple<Entity*, Entity*, Vector2D, float>>& collisionTuples) {
		for (auto& colTuple : collisionTuples) {
			Entity* entA = std::get<0>(colTuple);
			Entity* entB = std::get<1>(colTuple);
			Vector2D collisionNormal = std::get<2>(colTuple);
			float penetration = std::get<3>(colTuple);

			resolveCollision(*entA, *entB, collisionNormal, penetration);
		}
	}

	static void resolveCollisions() {
		std::vector<Entity*>& entities = EntityManager::getInstance().getEntities();
		std::vector<std::tuple<Entity*, Entity*, Vector2D, float>> collisionTuples;

		for (auto& thisEntity : entities) {
			// skip if this entity doesn't have any collider
			if (!thisEntity->HasComponent<BoxCollider2D>() && !thisEntity->HasComponent<CircleCollider2D>()) continue;

			for (auto& entity : entities) {
				// skip if checking same entity
				if (thisEntity->getId() == entity->getId()) continue;

				// skip if entity doesn't have any collider
				if (!entity->HasComponent<BoxCollider2D>() && !entity->HasComponent<CircleCollider2D>()) continue;

				// skip if neither entity have rigidbody component
				if (!thisEntity->HasComponent<Rigidbody2D>() && !entity->HasComponent<Rigidbody2D>()) continue;

				if (thisEntity->HasComponent<BoxCollider2D>() && entity->HasComponent<BoxCollider2D>()) {
					if (AABB(*thisEntity, *entity)) {
						std::pair<Vector2D, float> params = getAABBNormal(*thisEntity, *entity);
						Vector2D collisionNormal = params.first;
						float penetration = params.second;

						auto it = std::find_if(collisionTuples.begin(), collisionTuples.end(), [&](std::tuple<Entity*, Entity*, Vector2D, float> colTuple) {
							Entity* entA = std::get<0>(colTuple);
							Entity* entB = std::get<1>(colTuple);

							bool foundFirst = entA->getId() == thisEntity->getId() || entA->getId() == entity->getId();
							bool foundSecond = entB->getId() == thisEntity->getId() || entB->getId() == entity->getId();

							return foundFirst && foundSecond;
							});

						if (it != collisionTuples.end()) {
							continue;
						}

						collisionTuples.push_back({ thisEntity, entity, collisionNormal, penetration });

						//std::printf("Normal for Entity '%s' is (%d, %d)\n", thisEntity->getName().c_str(), (int)collisionNormal.x, (int)collisionNormal.y);
						//std::printf("Collision detected between entities %d and %d\n", thisEntity->getId(), entity->getId());
						break;
					}
				}

				if (thisEntity->HasComponent<CircleCollider2D>() && entity->HasComponent<CircleCollider2D>()) {
					if (CircleCircle(*thisEntity, *entity)) {
						std::pair<Vector2D, float> params = getCircleCircleNormal(*thisEntity, *entity);
						Vector2D collisionNormal = params.first;
						float penetration = params.second;

						resolveCollision(*thisEntity, *entity, collisionNormal, penetration);
						return;
					}
				}
				
			}
		}

		resolveCollisionTuples(collisionTuples);
	}
};