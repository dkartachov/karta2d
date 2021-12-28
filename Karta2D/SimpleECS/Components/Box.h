#pragma once
#include "Graphics.h"
#include "Transform2D.h"

class Box : public Component {
public:
	Box() {
		graphics = Graphics::Instance();
		size = 50 * oneVector;
		color = { 255, 255, 255, 255 };
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

	void setSize(float width, float height) {
		size = { width, height };
	}

	Vector2D getSize() {
		return size;
	}

	void update() override {
		
	}

	void render() override {
		graphics->drawBox(*transform, size);
	}

	void toString() override {
		std::printf("Box:\n");
		transform->toString();
		std::printf("Width = %f, Height = %f\n", size.x, size.y);
	}

private:
	Graphics* graphics;
	Transform2D* transform;
	Vector2D size;
	SDL_Color color;
};