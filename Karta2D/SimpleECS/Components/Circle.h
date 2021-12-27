#pragma once
#include "Graphics.h"
#include "Transform2D.h"

class Circle : public Component {
public:
	Circle() {
		radius = 50;
		color = { 255, 255, 255, 255 };
		transform = nullptr;
	}

	void init() override {
		if (!entity->HasComponent<Transform2D>()) {
			entity->AddComponent<Transform2D>();
		}

		transform = entity->GetComponent<Transform2D>();
	}

	void setRadius(int radius) {
		this->radius = radius;
	}

	void update() override {
		position = transform->getPosition();
	}

	void render() override {
		Graphics::Instance()->drawCircle(position, radius, false, color);
	}

	void toString() override {
		
	}

private:
	int radius;
	SDL_Color color;
	Transform2D* transform;
	Vector2D position;
};