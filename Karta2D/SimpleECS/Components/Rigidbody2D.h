#pragma once
#include "SimpleECS/SimpleECS.h"
#include "Transform2D.h"
#include "Timer.h"

class Rigidbody2D : public Component {
public:
	Rigidbody2D() {
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

	void update() override {
		transform->translate(Timer::Instance()->getDeltaTime() * velocity);
	}	

	void render() override {

	}

	void toString() override {

	}

private:
	float mass;
	Transform2D* transform;
	Vector2D velocity;
};