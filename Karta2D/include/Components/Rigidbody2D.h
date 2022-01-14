#pragma once
#include "SimpleECS.h"
#include "Transform2D.h"
#include "Timer.h"

class Rigidbody2D : public Component {
public:
	Rigidbody2D() {
		gravity = true;
		mass = 1;
		momentOfInertiaCOM = 0;
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

		if (!entity->HasComponent<BoxCollider2D>()) {
			entity->AddComponent<BoxCollider2D>();
		}

		boxCollider2D = entity->GetComponent<BoxCollider2D>();

		setMomentOfInertiaCOM();
	}

	// Set the mass of the rigid body in kg.
	void setMass(double mass) {
		this->mass = mass;

		setMomentOfInertiaCOM();
	}

	// Get the mass of the rigid body in kg.
	double getMass() const {
		return mass;
	}

	void setMomentOfInertiaCOM() {
		momentOfInertiaCOM = (1.0 / 12.0) * mass * boxCollider2D->getSize().MagnitudeSquared();
	}

	double getMomentOfInertiaCOM() const {
		return momentOfInertiaCOM;
	}
	
	// Set the velocity of the rigid body in m/s.
	void setVelocity(Vector2D velocity) {
		this->velocity = velocity;
	}

	// Get the velocity of the rigid body in m/s.
	Vector2D getVelocity() const {
		return velocity;
	}

	// Set the angular speed of the rigid body in deg/s (clockwise positive).
	void setAngularSpeed(double angularSpeed) {
		this->angularSpeed = angularSpeed;
	}

	// Get the angular speed of the rigid body in deg/s (clockwise positive).
	double getAngularSpeed() const {
		return angularSpeed;
	}
	
	// Set the acceleration of the rigid body in m/s/s.
	void setAcceleration(Vector2D acceleration) {
		this->acceleration = acceleration;
	}

	// Get the acceleration of the rigid body in m/s/s.
	Vector2D getAcceleration() const {
		return acceleration;
	}

	// Add a constant force to the rigid body in N.
	void addForce(Vector2D force) {
		acceleration = (force / mass);
	}

	// Enable/Disable gravity for the rigidbody.
	void setGravity(bool state) {
		gravity = state;
	}

	// Check if rigidbody has gravity enabled.
	bool hasGravity() const {
		return gravity;
	}

	void update() override {
		double deltaT = Timer::Instance()->getDeltaTime();

		// linear
		double deltaX = velocity.x * deltaT + 0.5 * acceleration.x * deltaT * deltaT;
		velocity.x += acceleration.x * deltaT;

		double deltaY = velocity.y * deltaT + 0.5 * (acceleration.y + (gravity ? g : 0)) * deltaT * deltaT;
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
	const double g = 9.81;
	bool gravity;
	double mass;
	double momentOfInertiaCOM;
	Transform2D* transform;
	BoxCollider2D* boxCollider2D;
	Vector2D velocity;
	double angularSpeed;
	Vector2D acceleration;
};