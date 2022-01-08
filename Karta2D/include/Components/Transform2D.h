#pragma once
#include "SimpleECS.h"
#include "Vector2D.h"

class Transform2D : public Component {
public:

	enum SPACE {local = 0, world};

	Transform2D() {
		scale = oneVector;
		position = zeroVector;
		rotation = 0.0;
	}

	void setScale(Vector2D scale) {
		if (entity->hasChildren()) {
			for (auto& child : entity->getChildren()) {
				Transform2D* childTransform = child->GetComponent<Transform2D>();

				Vector2D childScale = childTransform->getScale();
				Vector2D childRelPos = childTransform->getPosition() - position;

				Vector2D childRelScaledPos = childRelPos;

				if (childScale.x < scale.x || childScale.x > scale.x) {
					childRelScaledPos.x = (scale.x / childScale.x) * childRelScaledPos.x;
				}

				if (childScale.y < scale.y || childScale.y > scale.y) {
					childRelScaledPos.y = (scale.y / childScale.y) * childRelScaledPos.y;
				}

				childTransform->translate(childRelScaledPos - childRelPos);
				childTransform->setScale(scale);
			}
		}

		this->scale = scale;
	}

	// Get local/world space (world by default)
	Vector2D getScale(SPACE space = world) {
		return scale;
	}

	// Set position
	void setPosition(Vector2D position) {
		if (entity->hasChildren()) {
			for (auto& child : entity->getChildren()) {
				Transform2D* childTransform = child->GetComponent<Transform2D>();
				Vector2D childPosition = childTransform->getPosition();

				childTransform->setPosition(childPosition + (position - this->position));
			}
		}

		this->position = position;
	}

	// Get world position of entity. If entity has a parent, can specify local/world space.
	Vector2D getPosition(SPACE space = world) {
		return position;
	}

	// Set rotation (deg) to a specific angle (measured counter-clockwise)
	void setRotation(float angle) {
		if (entity->hasChildren()) {
			for (auto& child : entity->getChildren()) {
				child->GetComponent<Transform2D>()->setRotation(angle);

				Vector2D childPos = child->GetComponent<Transform2D>()->getPosition();
				childPos = childPos - position;
				childPos.rotateVector(angle);
				childPos = childPos + position;

				child->GetComponent<Transform2D>()->setPosition(childPos);
			}
		}

		this->rotation = -angle * DEG_TO_RAD;
	}

	// Get rotation in degrees
	float getRotation(SPACE space = world) {
		if (rotation * RAD_TO_DEG > 360 || rotation * RAD_TO_DEG < -360) rotation = 0;
		return rotation * RAD_TO_DEG;
	}

	// Translate entity by a certain amount
	void translate(Vector2D amount) {
		if (entity->hasChildren()) {
			for (auto& child : entity->getChildren()) {
				child->GetComponent<Transform2D>()->translate(amount);
			}
		}

		position += amount;
	}

	// Rotate (deg) by a certain amount (counter-clockwise positive)
	void rotate(float delta) {
		if (entity->hasChildren()) {
			for (auto& child : entity->getChildren()) {
				child->GetComponent<Transform2D>()->rotate(delta);

				Vector2D childPos = child->GetComponent<Transform2D>()->getPosition();
				childPos = childPos - position;
				childPos.rotateVector(delta);
				childPos = childPos + position;

				child->GetComponent<Transform2D>()->setPosition(childPos);
			}
		}
		
		rotation -= delta * DEG_TO_RAD;
	}

	void init() override {

	}

	void update() override {
		//std::printf("%s has child? : %s\n", entity->getName().c_str(), entity->hasChild() ? "true" : "false");
	}

	void render() override {

	}

	void toString() override {
		std::printf("Position: (%.2f, %.2f), Rotation: %.2f\n", position.x, position.y, rotation);
	}

private:
	Vector2D scale;
	Vector2D position;
	float rotation;
};