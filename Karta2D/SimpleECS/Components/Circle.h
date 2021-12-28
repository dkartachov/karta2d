#pragma once
#include "Graphics.h"
#include "Transform2D.h"

class Circle : public Component {
public:
	Circle() {
		radius = 50;
		thickness = 1;
		filled = false;
		outlineColor = fillColor = { 255, 255, 255, 255 };
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

	void setOutline(int thickness, SDL_Color outlineColor = { 255, 255, 255, 255 }) {
		this->thickness = thickness;
		this->outlineColor = outlineColor;
	}

	void fill(SDL_Color fillColor = { 255, 255, 255, 255 }) {
		filled = true;
		this->fillColor = fillColor;
	}

	void update() override {
		position = transform->getPosition();
	}

	void render() override {
		Graphics::Instance()->drawCircle(position, radius, thickness, outlineColor, filled, fillColor);
	}

	void toString() override {
		
	}

private:
	int radius;
	int thickness;
	bool filled;
	SDL_Color outlineColor, fillColor;
	Transform2D* transform;
	Vector2D position;
};