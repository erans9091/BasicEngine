#include "Point.h"
#include "Vector.h"
Point::Point(float x, float y, float z) : x(x), y(y), z(z) {  }

Point::Point() {
	x = 0, y = 0, z = 0;
}

Point::Point(const Point& p) {
	x = p.getX();
	y = p.getY();
	z = p.getZ();
}

Point::~Point() {} //no resources..

Vector Point::operator-(const Point & p) {
	return Vector(x - p.getX(), y - p.getY(), z - p.getZ());
}

Point Point::operator+(const Vector& v) {
	return Point(v.getX() + this->x, v.getY() + this->y, v.getZ() + this->z);
}

float Point::getX() const { return x; }
float Point::getY() const { return y; }
float Point::getZ() const { return z; }


