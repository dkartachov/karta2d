#pragma once
#include "Graphics.h"
#include "Transform2D.h"

class Square : public Component {
public:
	Square() {
		graphics = Graphics::Instance();

		color = { 255, 255, 255, 255 };
		size = 50 * oneVector;
		rect.w = size.x;
		rect.h = size.y;
		rect.x = rect.y = 0;
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
		rect.x = (int)transform->getPosition().x - rect.w / 2;
		rect.y = (int)transform->getPosition().y - rect.h / 2;
		rect.w = (int)size.x * transform->getScale().x;
		rect.h = (int)size.y * transform->getScale().y;
	}

	void render() override {
		//graphics->drawSquare(&rect, color);
		graphics->drawBox(*transform, size);
	}

	void toString() override {
		std::printf("Square:\n");
		transform->toString();
		std::printf("Width = %f, Height = %f\n", size.x, size.y);
	}

private:
	Graphics* graphics;
	Transform2D* transform;
	SDL_Rect rect;
	SDL_Color color;
	Vector2D size;
};