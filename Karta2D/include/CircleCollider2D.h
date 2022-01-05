#pragma once
#include "SimpleECS/SimpleECS.h"
#include "Graphics.h"
#include "Transform2D.h"

class CircleCollider2D : public Component {
public:
	CircleCollider2D() {
		transform = nullptr;
		radius = 50;
		show = true;
	}

	void init() override {
		if (!entity->HasComponent<Transform2D>()) {
			entity->AddComponent<Transform2D>();
		}

		transform = entity->GetComponent<Transform2D>();
	}

	void setRadius(float radius) {
		this->radius = radius;
	}

	float getRadius() const {
		return radius;
	}

	void update() override {

	}

	void render() override {
		if (show) {
			Graphics::Instance()->drawCircle(transform->getPosition(), radius, 1, { 0, 255, 0, 255 });
		}
	}

	void toString() override {

	}
private:
	Transform2D* transform;
	float radius;
	bool show;
};