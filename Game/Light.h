
#pragma once

#include "Vector.h"
#include "Point.h"
#include "Color.h"
class ObjectShape;
class Ray;

class Light {
public:
	Light(float r, float g, float b, float a);
	virtual ~Light() {};
	virtual Color CalcColorAtPoint(Point& p, Vector& normalAtPoint, ObjectShape * shape) = 0;
	virtual Ray occlusionRay(Point& intersectionWithObject)=0;
	virtual Ray reflectionRay(Point& intersectionWithObject ,  Vector & normalAtPoint) = 0;
	//virtual Ray refractionRay(Point& intersectionWithObject) = 0;
	Color getIntensity() const;
	static Point cameraPosition;
protected:
	//char type; // can be ambient, directional , or spot-light. a,d,s
	//Vector directionVec;
	//Point incidencePoint;
	Color intensity;
};

class Ambient : public Light {
public:
	~Ambient();
	Ambient(float r, float g, float b, float a) :Light(r, g, b, a) {};
	Ray occlusionRay(Point& intersectionWithObject) override;
	virtual Color CalcColorAtPoint(Point& p, Vector& normalAtPoint, ObjectShape * shape) override;
	Ray reflectionRay(Point& intersectionWithObject, Vector& normalAtPoint);
	//Ray refractionRay(Point& intersectionWithObject);
};

class Directional : public Light {
public:
	Directional(float r, float g, float b, float a, Vector lightDir);
	~Directional();
	Color CalcColorAtPoint(Point& p, Vector& normalAtPoint, ObjectShape * shape) override;
	Ray occlusionRay(Point& intersectionWithObject) override;
	Ray reflectionRay(Point& intersectionWithObject, Vector& normalAtPoint);
	//Ray refractionRay(Point& intersectionWithObject);
private:
	Vector lightDirection;
};


class Spotlight : public Light {
public:
	Spotlight(float r, float g, float b, float a, Vector lightDir, Point lightIncidenceP, float cosCutoffAngle);
	~Spotlight();
	Color CalcColorAtPoint(Point& p, Vector& normalAtPoint, ObjectShape * shape) override;
	Ray occlusionRay(Point& intersectionWithObject) override;
	Ray reflectionRay(Point& intersectionWithObject, Vector& normalAtPoint);
	//Ray refractionRay(Point& intersectionWithObject);
private:
	//float getCutoff();
	float cosCutoffAngle;
	Vector lightDirection;
	Point lightIncidencePoint;
	
};
