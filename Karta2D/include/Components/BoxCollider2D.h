#pragma once
#include "SimpleECS.h"
#include "Transform2D.h"
#include "Graphics.h"

class BoxCollider2D : public Component {
public:
	BoxCollider2D() {
		transform = nullptr;
		size = 1 * METERS_TO_PIXELS * oneVector;
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

	// Set the visibility of the box collider.
	void setVisibility(bool show) {
		this->show = show;
	}

	// Check whether the box collider is visible.
	bool isVisible() const {
		return show;
	}

	// Set the width and height of the box collider in meters.
	void setSize(double width, double height) {
		size.x = width * METERS_TO_PIXELS;
		size.y = height * METERS_TO_PIXELS;
	}

	// Get the size of the box collider in meters.
	Vector2D getSize() const {
		return size * PIXELS_TO_METERS;
	}

	double getDiag() const {
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
		double angle = transform->getRotation() * DEG_TO_RAD;
		double cosine = cos(angle);
		double sine = sin(angle);

		Vector2D n2 = { cosine, sine };
		Vector2D n4 = -n2;
		Vector2D n3 = { -sine, cosine };
		Vector2D n1 = -n3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		normals[3] = n4;

		// vertices calculation
		Vector2D position = transform->getPosition();
		double w = size.x * PIXELS_TO_METERS, h = size.y * PIXELS_TO_METERS;
		double rotation = transform->getRotation() * DEG_TO_RAD;
		double diagonal = sqrt(w * w + h * h);
		double diagonalAngle = atan2(h, w);

		Vector2D vertex1 = position - (diagonal / 2) * Vector2D(cos(diagonalAngle + rotation), sin(diagonalAngle + rotation));
		Vector2D vertex2 = position - (diagonal / 2) * Vector2D(-cos(diagonalAngle - rotation), sin(diagonalAngle - rotation));
		Vector2D vertex3 = position - (diagonal / 2) * Vector2D(-cos(diagonalAngle + rotation), -sin(diagonalAngle + rotation));
		Vector2D vertex4 = position - (diagonal / 2) * Vector2D(cos(diagonalAngle - rotation), -sin(diagonalAngle - rotation));

		vertices[0] = vertex1;
		vertices[1] = vertex2;
		vertices[2] = vertex3;
		vertices[3] = vertex4;
	}

	void render() override {
		if (show) {
			Graphics::Instance()->drawBox(*transform, size, false, { 0, 255, 0, 255 });
		}
	}

	void toString() override {

	}

public:
	double restitution;

private:
	Transform2D* transform;
	Vector2D size;
	std::vector<Vector2D> vertices;
	std::vector<Vector2D> normals;
	bool show;
};