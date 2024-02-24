
#pragma once

#include "Point.h"
#include "Vector.h";


class Ray {
public: 
	Vector directionVector;
	Point basePoint;

	Ray(Vector directionVec, Point basePoint);
	~Ray();
	Ray(const Ray & ray);

};
