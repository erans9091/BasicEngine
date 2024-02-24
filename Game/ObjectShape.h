

#pragma once

#include <vector>
#include "Color.h"
#include "Point.h"
#include "Vector.h"
class Ray;



class ObjectShape {
public:
	Color ambient;
	Color diffuse;
	Color specular;
	char textureType;
	ObjectShape(Color ambientCol, Color diffsueCol, Color specularCol, char t);
	virtual ~ObjectShape();
	virtual std::vector<Point> intersectionWithRay(Ray ray) = 0;
	virtual Vector normalAtPoint(Point p) = 0;
	virtual Color getDiffuse(Point interP)=0;
	bool isTrasp() { return textureType == 't'; };
	bool isReplect() { return textureType == 'r'; };
	bool isSolid() { return textureType == 'o'; };
};

class Sphere : public ObjectShape {
private:
	float radius;
	Point centerPoint;

public:

	Sphere(Color ambient, Color diffuse, Color specular, float radius, Point centerPoint, char t);
	std::vector<Point> intersectionWithRay(Ray ray);
	Vector normalAtPoint(Point p);
	Color getDiffuse(Point interP);
};



class Plane : public ObjectShape {
private:
	float signedDistanceFromOrigin;
	Vector planeNormal;
	Vector normalizedPlaneNormal;

public:
	Plane(Color ambient, Color diffuse, Color specular, float distFromOrigin, Vector planeNormal, char t);
	std::vector<Point> intersectionWithRay(Ray ray);
	Vector normalAtPoint(Point p);
	Color getDiffuse(Point interP);

};

