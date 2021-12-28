#pragma once
#include "SimpleECS/SimpleECS.h"
#include "Tools/Vector2D.h"

class Transform2D : public Component {
public:

	enum SPACE {local = 0, world};

	Transform2D() {
		scale = oneVector;
		position = zeroVector;
		rotation = 0.0;
	}

	void setScale(Vector2D scale) {
		this->scale = scale;
	}

	// Get local/world space (world by default)
	Vector2D getScale(SPACE space = world) {
		return scale;
	}

	// Set position
	void setPosition(Vector2D position) {
		this->position = position;
	}

	// Get world position of entity. If entity has a parent, can specify local/world space.
	Vector2D getPosition(SPACE space = world) {
		return position;
	}

	// Set rotation (deg) to a specific angle (measured counter-clockwise)
	void setRotation(float angle) {
		this->rotation = -angle * DEG_TO_RAD;
	}

	// Get rotation in degrees
	float getRotation(SPACE space = world) {
		return rotation * RAD_TO_DEG;
	}

	// Translate entity by a certain amount
	void translate(Vector2D amount) {
		position += amount;
	}

	// Rotate (deg) by a certain amount (counter-clockwise positive)
	void rotate(float delta) {
		rotation -= delta * DEG_TO_RAD;
	}

	void init() override {

	}

	void update() override {

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