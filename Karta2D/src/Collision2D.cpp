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
	Vector2D entAPos = entA.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS;
	float entADiag = entA.GetComponent<BoxCollider2D>()->getDiag();

	Vector2D entBPos = entB.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS;
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
	else if (maxA > minB && maxA < maxB && minA < minB) return maxA - minB;
	else return maxB - minA;
}

std::vector<Vector2D> Collision2D::getContactPoints(const Vector2D normal, const std::vector<Vector2D> entVertices) {
	// Entity A projections
	float proj1 = Vector2D::dot(normal, entVertices[0]);
	float proj2 = Vector2D::dot(normal, entVertices[1]);
	float proj3 = Vector2D::dot(normal, entVertices[2]);
	float proj4 = Vector2D::dot(normal, entVertices[3]);

	float min = std::min({ proj1, proj2, proj3, proj4 });

	std::vector<Vector2D> contactPoints;

	if (proj1 == min) {
		contactPoints.push_back(entVertices[0]);
	}

	if (proj2 == min) {
		contactPoints.push_back(entVertices[1]);
	}

	if (proj3 == min) {
		contactPoints.push_back(entVertices[2]);
	}

	if (proj4 == min) {
		contactPoints.push_back(entVertices[2]);
	}

	return contactPoints;
}

std::vector<Vector2D> Collision2D::getContactVertices(const Vector2D collisionNormal, const std::vector<Vector2D> entVertices, bool min) {
	// Entity projections
	float proj1 = Vector2D::dot(collisionNormal, entVertices[0]);
	float proj2 = Vector2D::dot(collisionNormal, entVertices[1]);
	float proj3 = Vector2D::dot(collisionNormal, entVertices[2]);
	float proj4 = Vector2D::dot(collisionNormal, entVertices[3]);

	float extremity = min ? std::min({ proj1, proj2, proj3, proj4 }) : std::max({ proj1, proj2, proj3, proj4 });

	std::vector<Vector2D> contactVertices;

	if (fabs(proj1 - extremity) < 2) {
		contactVertices.push_back(entVertices[0] * PIXELS_TO_METERS);
	}

	if (fabs(proj2 - extremity) < 2) {
		contactVertices.push_back(entVertices[1] * PIXELS_TO_METERS);
	}

	if (fabs(proj3 - extremity) < 2) {
		contactVertices.push_back(entVertices[2] * PIXELS_TO_METERS);
	}

	if (fabs(proj4 - extremity) < 2) {
		contactVertices.push_back(entVertices[3] * PIXELS_TO_METERS);
	}

	return contactVertices;
}

CollisionInfo Collision2D::getCollisionInfo(Entity& entA, Entity& entB) {
	Vector2D entAPos = entA.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS;
	float entADiag = entA.GetComponent<BoxCollider2D>()->getDiag();

	Vector2D entBPos = entB.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS;
	float entBDiag = entB.GetComponent<BoxCollider2D>()->getDiag();

	std::vector<Vector2D> entANormals, entBNormals;
	std::vector<Vector2D> entAVertices, entBVertices;

	if (entA.HasComponent<BoxCollider2D>()) {
		//entA.GetComponent<BoxCollider2D>()->update();
		entANormals = entA.GetComponent<BoxCollider2D>()->getNormals();
		entAVertices = entA.GetComponent<BoxCollider2D>()->getVertices();
	}

	if (entB.HasComponent<BoxCollider2D>()) {
		//entB.GetComponent<BoxCollider2D>()->update();
		entBNormals = entB.GetComponent<BoxCollider2D>()->getNormals();
		entBVertices = entB.GetComponent<BoxCollider2D>()->getVertices();
	}

	float dA = 1000;
	Vector2D nA = zeroVector;

	// check all entity A normals
	for (auto& normal : entANormals) {
		float overlap = getOverlap(normal, entAVertices, entBVertices);

		if (overlap < dA) {
			dA = overlap;
			nA = normal;
		}
	}

	float dB = 1000;
	Vector2D nB = zeroVector;

	// check all entity B normals
	for (auto& normal : entBNormals) {
		float overlap = getOverlap(normal, entAVertices, entBVertices);

		if (overlap < dB) {
			dB = overlap;
			nB = normal;
		}
	}

	float smallestOverlap;
	Vector2D collisionNormal;
	std::vector<Vector2D> contactVertices;

	if (dA < dB) {
		smallestOverlap = dA;
		collisionNormal = nA;

		Vector2D rAtoB = entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition();

		if (Vector2D::dot(rAtoB, collisionNormal) < 0) {
			collisionNormal = -collisionNormal;
		}

		contactVertices = getContactVertices(collisionNormal, entBVertices, true);
	}
	else {
		smallestOverlap = dB;
		collisionNormal = nB;

		Vector2D rAtoB = entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition();

		if (Vector2D::dot(rAtoB, collisionNormal) < 0) {
			collisionNormal = -collisionNormal;
		}

		contactVertices = getContactVertices(collisionNormal, entAVertices, false);
	}

	//printf("dA = %.2f and dB = %.2f\n", dA, dB);
	//printf("contact vertex = (%.2f, %.2f)\n", contactVertex.x, contactVertex.y);
	//printf("dot(nA, nB) = %.6f\n", Vector2D::dot(nA, nB));

	return CollisionInfo(smallestOverlap * PIXELS_TO_METERS, collisionNormal, contactVertices);
}

std::pair<Vector2D, float> Collision2D::getCircleCircleNormal(Entity& entA, Entity& entB) {
	Vector2D normal = (entB.GetComponent<Transform2D>()->getPosition() - entA.GetComponent<Transform2D>()->getPosition()).normalize();
	float penetration = fabs((entB.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS - entA.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS).Magnitude() - (entA.GetComponent<CircleCollider2D>()->getRadius() + entB.GetComponent<CircleCollider2D>()->getRadius()));
	return { normal, penetration };
}

std::pair<Vector2D, float> Collision2D::getBoxCircleNormal(Entity& box, Entity& circle) {
	Vector2D boxPos = box.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS;
	Vector2D boxSize = box.GetComponent<BoxCollider2D>()->getSize();

	Vector2D circlePos = circle.GetComponent<Transform2D>()->getPosition() * METERS_TO_PIXELS;
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

void Collision2D::resolveFullCollision(Entity& entityA, Entity& entityB, CollisionInfo collisionInfo) {
	bool entityAHasRb = entityA.HasComponent<Rigidbody2D>();
	bool entityBHasRb = entityB.HasComponent<Rigidbody2D>();

	Vector2D uA, cA, wAcA, vA;
	float wA = 0;
	
	if (entityAHasRb && !entityBHasRb && collisionInfo.collisionVertices.size() > 1) {
		// setting rotation like this makes absolutely no sense so should change
		entityA.GetComponent<Transform2D>()->setRotation(-entityB.GetComponent<Transform2D>()->getRotation());
		entityA.GetComponent<Transform2D>()->translate(-collisionInfo.penetration * collisionInfo.collisionNormal);
		entityA.GetComponent<Rigidbody2D>()->setAngularSpeed(0);

		Vector2D verticesVector = collisionInfo.collisionVertices[1] - collisionInfo.collisionVertices[0];
		verticesVector.normalize();

		Vector2D tangentialVector = Vector2D(-collisionInfo.collisionNormal.y, collisionInfo.collisionNormal.x);
		float cosTheta = Vector2D::dot(verticesVector, tangentialVector);
		float theta = acos(cosTheta) * RAD_TO_DEG;


		Vector2D entityAVel = entityA.GetComponent<Rigidbody2D>()->getVelocity();

		float velAlongNormal = Vector2D::dot(entityAVel, collisionInfo.collisionNormal);
		float restitution = 0.3;
		float j = -(1 + restitution) * velAlongNormal;

		float entityAInvMass = 1.0 / entityA.GetComponent<Rigidbody2D>()->getMass();

		j /= entityAInvMass;

		Vector2D impulse = j * collisionInfo.collisionNormal;
		entityA.GetComponent<Rigidbody2D>()->setVelocity(entityAVel + entityAInvMass * impulse);

		return;
	}

	if (entityAHasRb) {
		entityA.GetComponent<Transform2D>()->translate(-collisionInfo.penetration * collisionInfo.collisionNormal);
		entityA.GetComponent<BoxCollider2D>()->update();
		collisionInfo.collisionVertices[0] += -collisionInfo.penetration * collisionInfo.collisionNormal;

		uA = entityA.GetComponent<Rigidbody2D>()->getVelocity();
		wA = -entityA.GetComponent<Rigidbody2D>()->getAngularSpeed() * DEG_TO_RAD;
		cA = entityA.GetComponent<Transform2D>()->getPosition() - collisionInfo.collisionVertices[0];
		wAcA = wA * Vector2D(cA.y, -cA.x);

		vA = uA + wAcA;
	}

	printf("collision vertices = %d\n", collisionInfo.collisionVertices.size());
	printf("initial vA = %.6f\n", uA.Magnitude());
	printf("initial wA = %.6f\n", wA * RAD_TO_DEG);

	Vector2D uB, cB, wBcB, vB;
	float wB = 0;

	if (entityBHasRb) {
		uB = entityB.GetComponent<Rigidbody2D>()->getVelocity();
		wB = -entityB.GetComponent<Rigidbody2D>()->getAngularSpeed() * DEG_TO_RAD;
		cB = entityB.GetComponent<Transform2D>()->getPosition() - collisionInfo.collisionVertices[0];
		wBcB = wB * Vector2D(cB.y, -cB.x);

		vB = uB + wBcB;
	}

	Vector2D relV = vA - vB;
	float impV = Vector2D::dot(collisionInfo.collisionNormal, relV);
	float restitution = 0.3;

	float invMassA = entityAHasRb ? 1.0 / entityA.GetComponent<Rigidbody2D>()->getMass() : 0;
	float invMoiA = 0;
	float moiA;

	if (entityAHasRb) {
		moiA = (1.0 / 12.0) * (1.0 / invMassA) * (1 * 1 + 0.2 * 0.2);
		Vector2D n = collisionInfo.collisionNormal;
		invMoiA = (1.0 / moiA) * (n.x * cA.y - n.y * cA.x) * (n.x * cA.y - n.y * cA.x);
	}

	float invMassB = entityBHasRb ? 1.0 / entityB.GetComponent<Rigidbody2D>()->getMass() : 0;
	float invMoiB = 0;
	float moiB;

	if (entityBHasRb) {
		moiB = (1.0 / 12.0) * (1.0 / invMassB) * (6 * 6 + 0.5 * 0.5);
		Vector2D n = collisionInfo.collisionNormal;
		invMoiB = (1.0 / moiB) * (n.x * cB.y - n.y * cB.x) * (n.x * cB.y - n.y * cB.x);
	}

	float reducedMass = invMassA + invMoiA + invMassB + invMoiB;
	reducedMass = 1.0 / reducedMass;

	float impulse = (1.0 + restitution) * reducedMass * impV;

	if (entityAHasRb && entityBHasRb) {
		Vector2D deltaVA = -impulse * invMassA * collisionInfo.collisionNormal;
		Vector2D deltaVB = impulse * invMassB * collisionInfo.collisionNormal;

		float deltaWA = -impulse * (1.0 / moiA) * (cA.y * collisionInfo.collisionNormal.x - cA.x * collisionInfo.collisionNormal.y);
		float deltaWB = impulse * (1.0 / moiB) * (cB.y * collisionInfo.collisionNormal.x - cB.x * collisionInfo.collisionNormal.y);

		entityA.GetComponent<Transform2D>()->translate(-0.5 * collisionInfo.penetration * collisionInfo.collisionNormal);
		entityA.GetComponent<Rigidbody2D>()->setVelocity(uA + deltaVA);
		entityA.GetComponent<Rigidbody2D>()->setAngularSpeed((-1) * (wA + deltaWA) * RAD_TO_DEG);

		entityB.GetComponent<Transform2D>()->translate(0.5 * collisionInfo.penetration * collisionInfo.collisionNormal);
		entityB.GetComponent<Rigidbody2D>()->setVelocity(uB + deltaVB);
		entityB.GetComponent<Rigidbody2D>()->setAngularSpeed((-1) * (wB + deltaWB) * RAD_TO_DEG);

		return;
	}

	if (entityAHasRb) {
		Vector2D deltaVA = -impulse * invMassA * collisionInfo.collisionNormal;
		float deltaWA = -impulse * (1.0 / moiA) * (cA.y * collisionInfo.collisionNormal.x - cA.x * collisionInfo.collisionNormal.y);

		//printf("deltaV_A = %.6f\n", deltaVA.Magnitude());
		//printf("deltaW_A = %.6f\n", deltaWA * RAD_TO_DEG);

		//entityA.GetComponent<Transform2D>()->translate(-collisionInfo.penetration * collisionInfo.collisionNormal);
		entityA.GetComponent<Rigidbody2D>()->setVelocity(uA + deltaVA);
		entityA.GetComponent<Rigidbody2D>()->setAngularSpeed((-1) * (wA + deltaWA) * RAD_TO_DEG);
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

	std::vector<Vector2D> entityContactPoints = getContactPoints(collisionNormal, entity.GetComponent<BoxCollider2D>()->getVertices());
	//std::vector<Vector2D> thisEntityContactPoints = getContactPoints(collisionNormal, thisEntity.GetComponent<BoxCollider2D>()->getVertices());

	Vector2D contactToEntity;
	Vector2D entityFinalVel = entityVel - entityInvMass * impulse;
	float initialOmega = entityHasRb ? entity.GetComponent<Rigidbody2D>()->getAngularSpeed() : 0;
	float finalOmega = 0;

	if (entityContactPoints.size() < 2) {
		contactToEntity = entityContactPoints[0] * PIXELS_TO_METERS - entity.GetComponent<Transform2D>()->getPosition();
		float contactToEntityMagSqrd = contactToEntity.MagnitudeSquared();

		finalOmega += (contactToEntity.x * entityFinalVel.y - contactToEntity.y * entityFinalVel.x) / contactToEntityMagSqrd;
	}

	finalOmega *= RAD_TO_DEG;
	finalOmega += initialOmega;

	if (thisEntityHasRb && entityHasRb) {
		thisEntity.GetComponent<Transform2D>()->translate(0.5 * penetration * collisionNormal);
		thisEntity.GetComponent<Rigidbody2D>()->setVelocity(thisEntityVel + thisEntityInvMass * impulse);

		entity.GetComponent<Transform2D>()->translate(-0.5 * penetration * collisionNormal);
		entity.GetComponent<Rigidbody2D>()->setVelocity(entityVel - entityInvMass * impulse);
		entity.GetComponent<Rigidbody2D>()->setAngularSpeed(finalOmega);

		thisEntity.GetComponent<BoxCollider2D>()->update();
		entity.GetComponent<BoxCollider2D>()->update();

		return;
	}

	if (thisEntityHasRb) {
		thisEntity.GetComponent<Transform2D>()->translate(penetration * collisionNormal);
		thisEntity.GetComponent<Rigidbody2D>()->setVelocity(thisEntityVel + thisEntityInvMass * impulse);
		thisEntity.GetComponent<Rigidbody2D>()->setAngularSpeed(finalOmega);

		thisEntity.GetComponent<BoxCollider2D>()->update();
	}

	if (entityHasRb) {
		entity.GetComponent<Transform2D>()->translate(-penetration * collisionNormal);
		entity.GetComponent<Rigidbody2D>()->setVelocity(entityVel - entityInvMass * impulse);
		entity.GetComponent<Rigidbody2D>()->setAngularSpeed(finalOmega);

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
					CollisionInfo collisionInfo = getCollisionInfo(*thisEntity, *entity);

					resolveFullCollision(*thisEntity, *entity, collisionInfo);
					//resolveCollision(*thisEntity, *entity, collisionInfo.collisionNormal, collisionInfo.penetration);
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