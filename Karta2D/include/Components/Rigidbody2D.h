#pragma once
#include "SimpleECS.h"
#include "Transform2D.h"
#include "Timer.h"

class Rigidbody2D : public Component {
public:
	Rigidbody2D() {
		gravity = true;
		mass = 1;
		transform = nullptr;
		velocity = zeroVector;
		angularSpeed = 0;
		acceleration = zeroVector;
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

	// Set the angular speed of the object in deg/s (counter-clockwise positive).
	void setAngularSpeed(double angularSpeed) {
		this->angularSpeed = angularSpeed;
	}

	double getAngularSpeed() const {
		return angularSpeed;
	}

	void setAcceleration(Vector2D acceleration) {
		this->acceleration = acceleration;
	}

	Vector2D getAcceleration() const {
		return acceleration;
	}

	// Add a constant force to the body.
	void addForce(Vector2D force) {
		acceleration = force / mass;
	}

	void setGravity(bool state) {
		gravity = state;
	}

	bool hasGravity() const {
		return gravity;
	}

	void update() override {
		float deltaT = Timer::Instance()->getDeltaTime();

		// linear
		float deltaX = velocity.x * deltaT + 0.5 * acceleration.x * deltaT * deltaT;
		velocity.x += acceleration.x * deltaT;

		float deltaY = velocity.y * deltaT + 0.5 * ((double)acceleration.y + (gravity ? g : 0)) * deltaT * deltaT;
		velocity.y += (acceleration.y + (gravity ? g : 0)) * deltaT;

		transform->translate({ velocity.x * deltaT, velocity.y * deltaT });

		// angular
		transform->rotate(deltaT * angularSpeed);
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
	double angularSpeed;
	Vector2D acceleration;
};