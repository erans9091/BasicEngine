#include "Ray.h"
#include "Vector.h"
#include "Point.h"

Ray::Ray(Vector dir, Point p) : directionVector(dir), basePoint(p) {};

Ray::~Ray() {};

Ray::Ray(const Ray& otherRay) : directionVector(otherRay.directionVector), basePoint(otherRay.basePoint) {};