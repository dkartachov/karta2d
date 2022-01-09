#include "Collision2D.h"
#include <algorithm>

bool Collision2D::AABB(std::pair<Vector2D, Vector2D>& squareA, std::pair<Vector2D, Vector2D>& squareB) {
	Vector2D& squareAPos = squareA.first;
	Vector2D& squareABox = squareA.second;

	Vector2D squareBPos = squareB.first;
	Vector2D squareBBox = squareB.second;

	if (squareAPos.x + 0.5 * squareABox.x > squareBPos.x - 0.5 * squareBBox.x &&
		squareAPos.x - 0.5 * squareABox.x < squareBPos.x + 0.5 * squareBBox.x &&
		squareAPos.y + 0.5 * squareABox.y > squareBPos.y - 0.5 * squareBBox.y &&
		squareAPos.y - 0.5 * squareABox.y < squareBPos.y + 0.5 * squareBBox.y) {

		//std::printf("Collision detected between '%s' and '%s'\n", squareA.getName().c_str(), squareB.getName().c_str());
		return true;
	}

	return false;
}

bool Collision2D::BoxBox(Entity& boxA, Entity& boxB) {
	Vector2D boxAPos = boxA.GetComponent<Transform2D>()->getPosition();
	Vector2D boxABox = boxA.GetComponent<BoxCollider2D>()->getSize();

	Vector2D boxBPos = boxB.GetComponent<Transform2D>()->getPosition();
	Vector2D boxBBox = boxB.GetComponent<BoxCollider2D>()->getSize();

	std::pair<Vector2D, Vector2D> squareA = { boxAPos, boxABox };
	std::pair<Vector2D, Vector2D> squareB = { boxBPos, boxBBox };

	return AABB(squareA, squareB);
}

bool Collision2D::CircleCircle(Entity& thisEntity, Entity& entity) {
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

bool Collision2D::BoxCircle(Entity& box, Entity& circle) {
	Vector2D boxPos = box.GetComponent<Transform2D>()->getPosition();
	Vector2D boxBox = box.GetComponent<BoxCollider2D>()->getSize();

	Vector2D circlePos = circle.GetComponent<Transform2D>()->getPosition();
	float circleRadius = circle.GetComponent<CircleCollider2D>()->getRadius();

	std::pair<Vector2D, Vector2D> boxSquare = { boxPos, boxBox };
	std::pair<Vector2D, Vector2D> circleSquare = { circlePos, 2 * circleRadius * oneVector };

	if (AABB(boxSquare, circleSquare)) {
		bool collision = true;

		if (circlePos.x > boxPos.x + boxBox.x / 2) {
			if (circlePos.y > boxPos.y + boxBox.y / 2) { // bottom right
				Vector2D botRight = { boxPos.x + boxBox.x / 2, boxPos.y + boxBox.y / 2 };

				collision = (botRight - circlePos).MagnitudeSquared() <= circleRadius * circleRadius;
			}
			else if (circlePos.y < boxPos.y - boxBox.y / 2) { // top right
				Vector2D topRight = { boxPos.x + boxBox.x / 2, boxPos.y - boxBox.y / 2 };
				
				collision = (topRight - circlePos).MagnitudeSquared() <= circleRadius * circleRadius;
			}
		}
		else if (circlePos.x < boxPos.x - boxBox.x / 2) {
			if (circlePos.y > boxPos.y + boxBox.y / 2) { // bottom left
				Vector2D botLeft = { boxPos.x - boxBox.x / 2, boxPos.y + boxBox.y / 2 };

				collision = (botLeft - circlePos).MagnitudeSquared() <= circleRadius * circleRadius;
			}
			else if (circlePos.y < boxPos.y - boxBox.y / 2) { // top left
				Vector2D topLeft = { boxPos.x - boxBox.x / 2, boxPos.y - boxBox.y / 2 };

				collision = (topLeft - circlePos).MagnitudeSquared() <= circleRadius * circleRadius;
			}
		}

		if (collision) {
			//std::printf("Collision detected between '%s' and '%s'\n", box.getName().c_str(), circle.getName().c_str());
		}

		return collision;
	}

	return false;
}

bool Collision2D::overlap(const Vector2D normal, const std::vector<Vector2D>& entAVertices, const std::vector<Vector2D>& entBVertices) {
	// Entity A projections
	float projA1 = Vector2D::dot(normal, entAVertices[0]);
	float projA2 = Vector2D::dot(normal, entAVertices[1]);
	float projA3 = Vector2D::dot(normal, entAVertices[2]);
	float projA4 = Vector2D::dot(normal, entAVertices[3]);

	float minA = std::min({ projA1, projA2, projA3, projA4 });
	float maxA = std::max({ projA1, projA2, projA3, projA4 });

	// Entity B projections
	float projB1 = Vector2D::dot(normal, entBVertices[0]);
	float projB2 = Vector2D::dot(normal, entBVertices[1]);
	float projB3 = Vector2D::dot(normal, entBVertices[2]);
	float projB4 = Vector2D::dot(normal, entBVertices[3]);

	float minB = std::min({ projB1, projB2, projB3, projB4 });
	float maxB = std::max({ projB1, projB2, projB3, projB4 });

	return !(maxA <= minB || minA >= maxB);
}

bool Collision2D::satDetection(Entity& entA, Entity& entB) {
	Vector2D entAPos = entA.GetComponent<Transform2D>()->getPosition();
	float entADiag = entA.GetComponent<BoxCollider2D>()->getDiag();

	Vector2D entBPos = entB.GetComponent<Transform2D>()->getPosition();
	float entBDiag = entB.GetComponent<BoxCollider2D>()->getDiag();

	//if ((entAPos - entBPos).Magnitude() > entADiag + entBDiag) return false;

	std::vector<Vector2D> entANormals, entBNormals;
	std::vector<Vector2D> entAVertices, entBVertices;

	if (entA.HasComponent<BoxCollider2D>()) {
		entANormals = entA.GetComponent<BoxCollider2D>()->getNormals();
		entAVertices = entA.GetComponent<BoxCollider2D>()->getVertices();
	}
	
	if (entB.HasComponent<BoxCollider2D>()) {
		entBNormals = entB.GetComponent<BoxCollider2D>()->getNormals();
		entBVertices = entB.GetComponent<BoxCollider2D>()->getVertices();
	}

	bool collision = true;

	for (auto& normal : entANormals) {
		if (!overlap(normal, entAVertices, entBVertices)) {
			collision = false;
			break;
		}
	}

	if (collision) {
		for (auto& normal : entBNormals) {
			if (!overlap(normal, entAVertices, entBVertices)) {
				collision = false;
				break;
			}
		}
	}

	return collision;
}

//std::pair<Vector2D, float> Collision2D::getBoxBoxNormal(Entity& thisEntity, Entity& entity) {
//	Vector2D thisEntityPos = thisEntity.GetComponent<Transform2D>()->getPosition();
//	Vector2D thisEntityBox = thisEntity.GetComponent<BoxCollider2D>()->getSize();
//
//	Vector2D entityPos = entity.GetComponent<Transform2D>()->getPosition();
//	Vector2D entityBox = entity.GetComponent<BoxCollider2D>()->getSize();
//
//	double dx = 0;
//	if (entityPos.x > thisEntityPos.x) {
//		dx = fabs(((double)thisEntityPos.x - (double)entityPos.x) + 0.5 * ((double)thisEntityBox.x + (double)entityBox.x));
//	}
//	else {
//		dx = fabs(((double)thisEntityPos.x - (double)entityPos.x) - 0.5 * ((double)thisEntityBox.x + (double)entityBox.x));
//	}
//
//	double dy = 0;
//	if (entityPos.y > thisEntityPos.y) {
//		dy = fabs(((double)thisEntityPos.y - (double)entityPos.y) + 0.5 * ((double)thisEntityBox.y + (double)entityBox.y));
//	}
//	else {
//		dy = fabs(((double)thisEntityPos.y - (double)entityPos.y) - 0.5 * ((double)thisEntityBox.y + (double)entityBox.y));
//	}
//
//	if (dx <= dy) {
//		if (entityPos.x > thisEntityPos.x) {
//			return { Vector2D(-1, 0), dx };
//		}
//		else {
//			return { Vector2D(1, 0), dx };
//		}
//	}
//	else {
//		if (entityPos.y > thisEntityPos.y) {
//			return { Vector2D(0, -1), dy };
//		}
//		else {
//			return { Vector2D(0, 1), dy };
//		}
//	}
//}

float Collision2D::getOverlap(const Vector2D normal, const std::vector<Vector2D> entAVertices, const std::vector<Vector2D> entBVertices) {
	// Entity A projections
	float projA1 = Vector2D::dot(normal, entAVertices[0]);
	float projA2 = Vector2D::dot(normal, entAVertices[1]);
	float projA3 = Vector2D::dot(normal, entAVertices[2]);
	float projA4 = Vector2D::dot(normal, entAVertices[3]);

	float minA = std::min({ projA1, projA2, projA3, projA4 });
	float maxA = std::max({ projA1, projA2, projA3, projA4 });

	// Entity B projections
	float projB1 = Vector2D::dot(normal, entBVertices[0]);
	float projB2 = Vector2D::dot(normal, entBVertices[1]);
	float projB3 = Vector2D::dot(normal, entBVertices[2]);
	float projB4 = Vector2D::dot(normal, entBVertices[3]);

	float minB = std::min({ projB1, projB2, projB3, projB4 });
	float maxB = std::max({ projB1, projB2, projB3, projB4 });

	if (minA < minB && maxA > maxB) return maxB - minB; // if B fully contained within A, return full B
	else if (minB < minA && maxB > maxA) return maxA - minA; // if A fully contained within B, return full A
	else if (maxA > minB && minA < minB) return maxA - minB;
	else return maxB - minA;
}

std::pair<Vector2D, float> Collision2D::getSATNormal(Entity& entA, Entity& entB) {
	Vector2D entAPos = entA.GetComponent<Transform2D>()->getPosition();
	float entADiag = entA.GetComponent<BoxCollider2D>()->getDiag();

	Vector2D entBPos = entB.GetComponent<Transform2D>()->getPosition();
	float entBDiag = entB.GetComponent<BoxCollider2D>()->getDiag();

	std::vector<Vector2D> entANormals, entBNormals;
	std::vector<Vector2D> entAVertices, entBVertices;

	if (entA.HasComponent<BoxCollider2D>()) {
		entA.GetComponent<BoxCollider2D>()->update();
		entANormals = entA.GetComponent<BoxCollider2D>()->getNormals();
		entAVertices = entA.GetComponent<BoxCollider2D>()->getVertices();
	}

	if (entB.HasComponent<BoxCollider2D>()) {
		entB.GetComponent<BoxCollider2D>()->update();
		entBNormals = entB.GetComponent<BoxCollider2D>()->getNormals();
		entBVertices = entB.GetComponent<BoxCollider2D>()->getVertices();
	}

	float smallestOverlap = 1000;
	Vector2D collisionNormal = zeroVector;

	// check all entity A normals
	for (auto& normal : entANormals) {
		float overlap = getOverlap(normal, entAVertices, entBVertices);

		if (overlap < smallestOverlap) {
			smallestOverlap = overlap;
			collisionNormal = normal;
		}
	}

	// check all entity B normals
	for (auto& normal : entBNormals) {
		float overlap = getOverlap(normal, entAVertices, entBVertices);

		if (overlap < smallestOverlap) {
			smallestOverlap = overlap;
			collisionNormal = normal;
		}
	}

	return { collisionNormal, smallestOverlap };
}

std::pair<Vector2D, float> Collision2D::getCircleCircleNormal(Entity& entA, Entity& entB) {
	Vector2D normal = (entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition()).normalize();
	float penetration = fabs((entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition()).Magnitude() - (entA.GetComponent<CircleCollider2D>()->getRadius() + entB.GetComponent<CircleCollider2D>()->getRadius()));
	return { normal, penetration };
}

std::pair<Vector2D, float> Collision2D::getBoxCircleNormal(Entity& box, Entity& circle) {
	Vector2D boxPos = box.GetComponent<Transform2D>()->getPosition();
	Vector2D boxSize = box.GetComponent<BoxCollider2D>()->getSize();

	Vector2D circlePos = circle.GetComponent<Transform2D>()->getPosition();
	float radius = circle.GetComponent<CircleCollider2D>()->getRadius();

	Vector2D normal;
	float penetration;

	double dx = 0;
	if (circlePos.x > boxPos.x) {
		dx = fabs(((double)boxPos.x - (double)circlePos.x) + 0.5 * ((double)boxSize.x + (double)2 * radius));
	}
	else {
		dx = fabs(((double)boxPos.x - (double)circlePos.x) - 0.5 * ((double)boxSize.x + (double)2 * radius));
	}

	double dy = 0;
	if (circlePos.y > boxPos.y) {
		dy = fabs(((double)boxPos.y - (double)circlePos.y) + 0.5 * ((double)boxSize.y + (double)2 * radius));
	}
	else {
		dy = fabs(((double)boxPos.y - (double)circlePos.y) - 0.5 * ((double)boxSize.y + (double)2 * radius));
	}

	if (dx <= dy) {
		if (circlePos.x > boxPos.x) {
			return { Vector2D(-1, 0), dx };
		}
		else {
			return { Vector2D(1, 0), dx };
		}
	}
	else {
		if (circlePos.y > boxPos.y) {
			return { Vector2D(0, -1), dy };
		}
		else {
			return { Vector2D(0, 1), dy };
		}
	}
}

void Collision2D::resolveCollision(Entity& thisEntity, Entity& entity, Vector2D collisionNormal, float penetration) {
	bool thisEntityHasRb = thisEntity.HasComponent<Rigidbody2D>();
	bool entityHasRb = entity.HasComponent<Rigidbody2D>();

	Vector2D thisEntityVel = thisEntityHasRb ? thisEntity.GetComponent<Rigidbody2D>()->getVelocity() : zeroVector;
	Vector2D entityVel = entityHasRb ? entity.GetComponent<Rigidbody2D>()->getVelocity() : zeroVector;

	Vector2D relVelocity = thisEntityVel - entityVel;
	float velAlongNormal = Vector2D::dot(relVelocity, collisionNormal);

	if (velAlongNormal > 0) return;

	float restitution = (thisEntity.GetComponent<BoxCollider2D>()->restitution + entity.GetComponent<BoxCollider2D>()->restitution) / 2;

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

		thisEntity.GetComponent<BoxCollider2D>()->update();
		entity.GetComponent<BoxCollider2D>()->update();

		return;
	}

	if (thisEntityHasRb) {
		thisEntity.GetComponent<Transform2D>()->translate(penetration * collisionNormal);
		thisEntity.GetComponent<Rigidbody2D>()->setVelocity(thisEntityVel + thisEntityInvMass * impulse);

		thisEntity.GetComponent<BoxCollider2D>()->update();
	}

	if (entityHasRb) {
		entity.GetComponent<Transform2D>()->translate(-penetration * collisionNormal);
		entity.GetComponent<Rigidbody2D>()->setVelocity(entityVel - entityInvMass * impulse);

		entity.GetComponent<BoxCollider2D>()->update();
	}
}

void Collision2D::resolveCollisions() {
	std::vector<Entity*>& entities = EntityManager::getInstance().getEntities();

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

			// DETECT AND RESOLVE BOX-BOX COLLISIONS //
			if (thisEntity->HasComponent<BoxCollider2D>() && entity->HasComponent<BoxCollider2D>()) {
				if (satDetection(*thisEntity, *entity)) {
					std::pair<Vector2D, float> params = getSATNormal(*thisEntity, *entity);
					Vector2D collisionNormal = params.first;
					float penetration = params.second;

					resolveCollision(*thisEntity, *entity, collisionNormal, penetration);
				}
			}

			// DETECT AND RESOLVE CIRCLE-CIRCLE COLLISIONS //
			if (thisEntity->HasComponent<CircleCollider2D>() && entity->HasComponent<CircleCollider2D>()) {
				if (CircleCircle(*thisEntity, *entity)) {
					std::pair<Vector2D, float> params = getCircleCircleNormal(*thisEntity, *entity);
					Vector2D collisionNormal = params.first;
					float penetration = params.second;

					resolveCollision(*thisEntity, *entity, collisionNormal, penetration);
				}
			}

			// DETECT AND RESOLVE BOX-CIRCLE COLLISIONS // TODO: Improve. A lot lol
			if (thisEntity->HasComponent<BoxCollider2D>() && entity->HasComponent<CircleCollider2D>()) {
				if (BoxCircle(*thisEntity, *entity)) {
					std::pair<Vector2D, float> params = getBoxCircleNormal(*thisEntity, *entity);
					Vector2D collisionNormal = params.first;
					float penetration = params.second;

					resolveCollision(*thisEntity, *entity, collisionNormal, penetration);
				}
			}
		}
	}
}