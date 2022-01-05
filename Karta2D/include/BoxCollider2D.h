#pragma once
#include "SimpleECS.h"
#include "Transform2D.h"
#include "Graphics.h"

class BoxCollider2D : public Component {
public:
	BoxCollider2D() {
		transform = nullptr;
		size = 50 * oneVector;
		show = true;
	}

	void init() override {
		if (!entity->HasComponent<Transform2D>()) {
			entity->AddComponent<Transform2D>();
		}

		transform = entity->GetComponent<Transform2D>();
	}

	void setVisibility(bool show) {
		this->show = show;
	}

	bool isVisible() const {
		return show;
	}

	void setSize(int width, int height) {
		size.x = width;
		size.y = height;
	}

	Vector2D getSize() const {
		return size;
	}

	void update() override {

	}

	void render() override {
		if (show) {
			Graphics::Instance()->drawBox(*transform, size, false, { 0, 255, 0, 255 });
		}
	}

	void toString() override {

	}
private:
	Transform2D* transform;
	Vector2D size;
	bool show;
};