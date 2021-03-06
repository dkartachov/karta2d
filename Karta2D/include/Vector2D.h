#pragma once
#include <math.h>

constexpr double PI = 3.1415926535897932384626433832795;
constexpr double DEG_TO_RAD = PI / 180.0;
constexpr double RAD_TO_DEG = 180.0 / PI;
constexpr double METERS_TO_PIXELS = 100.0;
constexpr double PIXELS_TO_METERS = 1.0 / METERS_TO_PIXELS;

struct Vector2D {
	double x, y;

	Vector2D() {
		x = y = 0;
	}

	Vector2D(double x, double y) {
		this->x = x;
		this->y = y;
	}

	double MagnitudeSquared() {
		return x * x + y * y;
	}

	double Magnitude() {
		return sqrt(MagnitudeSquared());
	}

	inline static double dot(const Vector2D& a, const Vector2D& b) {
		return (a.x * b.x + a.y * b.y);
	}

	Vector2D normalize() {
		return Vector2D(x / Magnitude(), y / Magnitude());
	}

	inline static Vector2D perpendicular(Vector2D& v) {
		return Vector2D(-v.y, v.x);
	}

	// Rotate vector by an angle in degrees (clockwise positive)
	inline void rotateVector(double angle) {
		angle = angle * DEG_TO_RAD;

		double cosine = cos(angle);
		double sine = sin(angle);
		double newX = x * cosine - y * sine;
		double newY = x * sine + y * cosine;

		x = newX;
		y = newY;
	}

	inline Vector2D operator+ (const Vector2D& v) {
		return Vector2D(x + v.x, y + v.y);
	}

	inline Vector2D operator+= (const Vector2D& v) {
		return Vector2D(x += v.x, y += v.y);
	}

	inline Vector2D operator*= (const float& c) {
		return Vector2D(x *= c, y *= c);
	}

	inline Vector2D operator- (const Vector2D& v) {
		return Vector2D(x - v.x, y - v.y);
	}

	inline Vector2D operator- () {
		return Vector2D(-x, -y);
	}
};

inline double operator* (const Vector2D& u, const Vector2D& v) {
	return u.x * v.y - u.y * v.x;
}

inline Vector2D operator* (const double& c, const Vector2D& v) {
	return Vector2D(c * v.x, c * v.y);
}

inline Vector2D operator* (const Vector2D& v, const double& c) {
	return operator*(c, v);
}

inline Vector2D operator/ (const Vector2D& v, const double& c) {
	return Vector2D(v.x / c, v.y / c);
}

const Vector2D zeroVector = { 0.0, 0.0 };
const Vector2D oneVector = { 1.0, 1.0 };