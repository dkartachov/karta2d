#pragma once
#include <math.h>

constexpr double PI = 3.14159265;
constexpr double DEG_TO_RAD = PI / 180.0;

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

	inline Vector2D operator+ (const Vector2D& v) {
		return Vector2D(x + v.x, y + v.y);
	}

	inline Vector2D operator- (const Vector2D& v) {
		return Vector2D(x - v.x, y - v.y);
	}
};

inline Vector2D operator* (const float& c, const Vector2D& v) {
	return Vector2D(c * v.x, c * v.y);
}

// Rotate a vector by an angle in degrees.
inline Vector2D rotateVector(Vector2D& v, float angle) {
	angle *= DEG_TO_RAD;
	return Vector2D(v.x * cos(angle) - v.y * sin(angle), v.x * sin(angle) + v.y * cos(angle));
}

const Vector2D zeroVector = { 0.0, 0.0 };
const Vector2D oneVector = { 1.0, 1.0 };