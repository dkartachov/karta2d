#pragma once
#include "Graphics.h"
#include "Transform2D.h"

class Box : public Component {
public:
	Box() {
		graphics = Graphics::Instance();
		size = 1 * METERS_TO_PIXELS * oneVector;
		width = size.x;
		height = size.y;
		color = { 255, 255, 255, 255 };
		filled = false;
		transform = nullptr;
	}

	void init() override {

		if (!entity->HasComponent<Transform2D>()) {
			entity->AddComponent<Transform2D>();
		}

		transform = entity->GetComponent<Transform2D>();
	}

	void setColor(int r, int g, int b, int a = 255) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void fill() {
		filled = true;
	}
	
	// Set width and height of the box in meters
	void setSize(float width, float height) {
		width *= METERS_TO_PIXELS;
		height *= METERS_TO_PIXELS;

		size = Vector2D(width, height);
		this->width = width;
		this->height = height;
	}

	Vector2D getSize() {
		return Vector2D(width, height);
	}

	void update() override {
		width = size.x * transform->getScale().x;
		height = size.y * transform->getScale().y;
	}

	void render() override {
		graphics->drawBox(*transform, Vector2D(width, height), filled, color);
	}

	void toString() override {
		std::printf("Box:\n");
		transform->toString();
		std::printf("Width = %.2f, Height = %.2f\n", width, height);
	}

private:
	Graphics* graphics;
	Transform2D* transform;
	Vector2D size;
	SDL_Color color;
	bool filled;
	float width, height;
};