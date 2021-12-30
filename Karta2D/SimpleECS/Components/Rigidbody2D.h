#pragma once
#include "SimpleECS/SimpleECS.h"
#include "Transform2D.h"
#include "Timer.h"

class Rigidbody2D : public Component {
public:
	Rigidbody2D() {
		transform = nullptr;
		velocity = zeroVector;
	}

	void init() override {
		if (!entity->HasComponent<Transform2D>()) {
			entity->AddComponent<Transform2D>();
		}

		transform = entity->GetComponent<Transform2D>();
	}

	void setVelocity(Vector2D velocity) {
		this->velocity = velocity;
	}

	void update() override {
		transform->translate(Timer::Instance()->getDeltaTime() * velocity);
	}	

	void render() override {

	}

	void toString() override {

	}

private:
	Transform2D* transform;
	Vector2D velocity;
};