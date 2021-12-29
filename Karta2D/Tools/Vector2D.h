#pragma once
#include <math.h>

constexpr double PI = 3.1415926535897932384626433832795;
constexpr double DEG_TO_RAD = PI / 180.0;
constexpr double RAD_TO_DEG = 180.0 / PI;

struct Vector2D {
	float x, y;

	Vector2D() {
		x = y = 0;
	}

	Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	}

	float MagnitudeSquared() {
		return x * x + y * y;
	}

	float Magnitude() {
		return (float) sqrt(MagnitudeSquared());
	}

	// Rotate vector by an angle in degrees (counter-clockwise positive)
	inline void rotateVector(float angle) {
		angle = angle * DEG_TO_RAD;

		double cosine = cos(angle);
		double sine = sin(angle);
		float newX = x * cosine + y * sine;
		float newY = -x * sine + y * cosine;

		x = newX;
		y = newY;
	}

	inline Vector2D operator+ (const Vector2D& v) {
		return Vector2D(x + v.x, y + v.y);
	}

	inline Vector2D operator+= (const Vector2D& v) {
		//x += v.x;
		//y += v.y;
		return Vector2D(x += v.x, y += v.y);
	}

	inline Vector2D operator- (const Vector2D& v) {
		return Vector2D(x - v.x, y - v.y);
	}
};

inline Vector2D operator* (const float& c, const Vector2D& v) {
	return Vector2D(c * v.x, c * v.y);
}

const Vector2D zeroVector = { 0.0, 0.0 };
const Vector2D oneVector = { 1.0, 1.0 };