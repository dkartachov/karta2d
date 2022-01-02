#pragma once
#include "SimpleECS/SimpleECS.h"
#include "Transform2D.h"
#include "Timer.h"

class Rigidbody2D : public Component {
public:
	Rigidbody2D() {
		gravity = true;
		mass = 1;
		transform = nullptr;
		velocity = zeroVector;
	}

	void init() override {
		if (!entity->HasComponent<Transform2D>()) {
			entity->AddComponent<Transform2D>();
		}

		transform = entity->GetComponent<Transform2D>();
	}

	void setMass(float mass) {
		this->mass = mass;
	}

	float getMass() const {
		return mass;
	}

	void setVelocity(Vector2D velocity) {
		this->velocity = velocity;
	}

	Vector2D getVelocity() const {
		return velocity;
	}

	void setGravity(bool state) {
		gravity = state;
	}

	bool hasGravity() const {
		return gravity;
	}

	void update() override {
		float deltaT = Timer::Instance()->getDeltaTime();

		if (gravity) {
			float deltaY = velocity.y * deltaT + 0.5 * g * deltaT * deltaT;
			velocity.y += g * deltaT;
		}

		transform->translate({ velocity.x * deltaT, velocity.y * deltaT });
	}	

	void render() override {

	}

	void toString() override {

	}

private:
	const float g = 150;
	bool gravity;
	float mass;
	Transform2D* transform;
	Vector2D velocity;
};