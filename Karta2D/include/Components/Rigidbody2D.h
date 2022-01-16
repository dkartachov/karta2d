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
		momentOfInertiaConstraint = 0;
		transform = nullptr;
		velocity = zeroVector;
		angularSpeed = 0;
		acceleration = zeroVector;
		constrained = false;
		constraint = zeroVector;
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

		if (constrained) {
			this->angularSpeed = (transform->getPosition() - constraint) * this->velocity * RAD_TO_DEG;
		}
	}

	// Get the velocity of the rigid body in m/s.
	Vector2D getVelocity() const {
		return velocity;
	}

	// Set the angular speed of the rigid body in deg/s (clockwise positive).
	void setAngularSpeed(double angularSpeed) {
		this->angularSpeed = angularSpeed;

		if (constrained) {
			Vector2D constraintToRb = transform->getPosition() - constraint;
			this->velocity = this->angularSpeed * Vector2D(-constraintToRb.y, constraintToRb.x);
		}
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

	// Constrain rigidbody to a point in space relative to its position, in meters.
	void setContraint(Vector2D point) {
		constraint = transform->getPosition() + point;
		constrained = true;

		// moment of inertia about constraint
		momentOfInertiaConstraint = momentOfInertiaCOM + mass * (transform->getPosition() - constraint).MagnitudeSquared();
	}

	// Check if the rigid body is constrained.
	bool isConstrained() const {
		return constrained;
	}

	void update() override {
		double deltaT = Timer::Instance()->getDeltaTime();

		if (constrained) {
			Vector2D constraintToRb = transform->getPosition() - constraint;

			double theta0 = atan2(constraintToRb.y, constraintToRb.x);

			double currentRotation = transform->getRotation();

			double rel = fabs(velocity.Magnitude() / constraintToRb.Magnitude()) - fabs(angularSpeed * DEG_TO_RAD);

			double omega0 = angularSpeed * DEG_TO_RAD;


			Vector2D velocity0 = omega0 * Vector2D(-constraintToRb.y, constraintToRb.x);

			// Runge-Kutta 2nd order
			double k1 = omega0;
			double l1 = gravity ? (1.0 / momentOfInertiaConstraint) * mass * g * constraintToRb.Magnitude() * cos(theta0) : 0.0;
			double k2 = omega0 + deltaT * l1;
			double l2 = gravity ? (1.0 / momentOfInertiaConstraint) * mass * g * constraintToRb.Magnitude() * cos(theta0 + deltaT * omega0) : 0.0;

			double theta = theta0 + 0.5 * deltaT * (k1 + k2);
			double omega = omega0 + 0.5 * deltaT * (l1 + l2);

			velocity = constraintToRb.Magnitude() * omega * Vector2D(-sin(theta), cos(theta));
			angularSpeed = omega * RAD_TO_DEG;

			Vector2D positionDelta = constraint + constraintToRb.Magnitude() * Vector2D(cos(theta), sin(theta));

			transform->setPosition(positionDelta);
			transform->setRotation((theta - theta0) * RAD_TO_DEG + currentRotation);

			return;
		}

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
	double momentOfInertiaConstraint;
	Transform2D* transform;
	BoxCollider2D* boxCollider2D;
	Vector2D velocity;
	double angularSpeed;
	Vector2D acceleration;
	bool constrained;
	Vector2D constraint;
};