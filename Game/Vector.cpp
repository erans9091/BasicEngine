#pragma once
#include "Vector.h"

#include <math.h>



Vector::Vector(float x, float y, float z) : x(x) , y(y) , z(z) {}

Vector::Vector() : x(0) ,y(0), z(0) {}

Vector & Vector::operator=(const Vector& otherVec) {
	x = otherVec.getX();
	y=  otherVec.getY();
	z = otherVec.getZ();
	return *this;
}

Vector::~Vector() {}

Vector::Vector(const Vector & otherVec) : x(otherVec.getX()) , y(otherVec.getY()), z(otherVec.getZ()) { }

float Vector::getX() const { return this->x; }
float Vector::getY() const { return this->y; }
float Vector::getZ() const { return this->z; }

Vector Vector::Negate() {
	return Vector(-this->x, -this->y, -this->z);
}

float Vector::operator*() { //dot product of a vector with itself - ie V^2
	return ((x * x) + (y * y) + (z * z));
}

float Vector::Norm() {
	return sqrtf( (x * x) + (y * y) + (z * z) );
}

void Vector::NormalizeVec() {
	float norm = this->Norm();
	x = x / norm;
	y = y / norm;
	z = z / norm;
}

int Vector::isNormalized() {
	return this->Norm() == 1;
}


Vector Vector::operator - (const Vector& otherVec) {
	return Vector(x - otherVec.getX(), y - otherVec.getY(), z - otherVec.getZ());
}


float Vector::operator * (Vector v1) { //returns the dot product!!
	return (x * v1.getX()) + (y * v1.getY()) + (z * v1.getZ());
}

Vector Vector::operator*(float scaleValue) {
	return Vector(x * scaleValue, y * scaleValue, z * scaleValue);
}