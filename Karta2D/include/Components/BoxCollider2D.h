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
		restitution = 0.5;

		normals.push_back(zeroVector);
		normals.push_back(zeroVector);
		normals.push_back(zeroVector);
		normals.push_back(zeroVector);

		vertices.push_back(zeroVector);
		vertices.push_back(zeroVector);
		vertices.push_back(zeroVector);
		vertices.push_back(zeroVector);
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

	float getDiag() const {
		return sqrt(0.25 * (size.x * size.x + size.y * size.y));
	}

	std::vector<Vector2D> getVertices() const {
		return vertices;
	}

	std::vector<Vector2D> getNormals() const {
		return normals;
	}

	void update() override {
		// normals calculation
		float angle = transform->getRotation() * DEG_TO_RAD;
		float cosine = cos(angle);
		float sine = sin(angle);

		Vector2D n1 = { cosine, sine };
		Vector2D n2 = -n1;
		Vector2D n3 = { -sine, cosine };
		Vector2D n4 = -n3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		normals[3] = n4;

		// vertices calculation
		double x = transform->getPosition().x, y = transform->getPosition().y;
		double w = size.x, h = size.y;

		double diag = sqrt(0.25 * (w * w + h * h));
		double diagAngle = atan2(h, w);
		double theta = transform->getRotation() * DEG_TO_RAD;

		double x1 = x - diag * cos(theta + diagAngle);
		double y1 = y - diag * sin(theta + diagAngle);
		Vector2D v1 = { (float)x1, (float)y1 };

		double x2 = x1 + w * cos(theta);
		double y2 = y1 + w * sin(theta);
		Vector2D v2 = { (float)x2, (float)y2 };

		double x3 = 2 * x - x1;
		double y3 = 2 * y - y1;
		Vector2D v3 = { (float)x3, (float)y3 };

		double x4 = x1 - h * sin(theta);
		double y4 = y1 + h * cos(theta);
		Vector2D v4 = { (float)x4, (float)y4 };

		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
		vertices[3] = v4;

		//std::printf("%s normal vectors: (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", entity->getName().c_str(), n1.x, n1.y, n2.x, n2.y, n3.x, n3.y, n4.x, n4.y);
	}

	void render() override {
		if (show) {
			Graphics::Instance()->drawBox(*transform, size, false, { 0, 255, 0, 255 });
		}
	}

	void toString() override {

	}

public:
	float restitution;

private:
	Transform2D* transform;
	Vector2D size;
	std::vector<Vector2D> vertices;
	std::vector<Vector2D> normals;
	bool show;
};