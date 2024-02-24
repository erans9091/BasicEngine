#include "ObjectShape.h"
#include "Color.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#define _USE_MATH_DEFINES
#include <math.h>


ObjectShape::ObjectShape(Color ambientCol, Color diffsueCol, Color specularCol, char t) : ambient(ambientCol), diffuse(diffsueCol), specular(specularCol)
{
	textureType = t;
}; //add copy constructors in Color class

Sphere::Sphere(Color ambient, Color diffuse, Color specular, float radius, Point centerPoint, char t) :
	ObjectShape(ambient, diffuse, specular, t), radius(radius), centerPoint(centerPoint) {};

Vector Sphere::normalAtPoint(Point p) {
	Vector normalAtP = p - this->centerPoint;
	normalAtP.NormalizeVec();
	return normalAtP;
}


Color Sphere::getDiffuse(Point interP) {
	return diffuse;

}

std::vector<Point> Sphere::intersectionWithRay(Ray ray) { //let C= center point, V= direction vector of ray , P - initial point of ray . squaring a vector means it's squared norm
	std::vector<Point> intersections; //

	//solutions to the quadratic equation
	float t1 = 0;
	float t2 = 0;

	//SOLVE QUADRATIC EQUATION
	Vector pointVec = ray.basePoint - Point();
	Vector centerPointVec = this->centerPoint - Point();
	float a = *ray.directionVector; // V^2 - note it's always positive!
	float b = 2 * (ray.directionVector * pointVec) - 2 * (centerPointVec * ray.directionVector); // 2* (V * S) - 2 (C * V)
	float c = *pointVec.operator-(centerPointVec) -(this->radius * this->radius); // (S-C)^ 2 -r^2. where r is the sphere's radius

	float b_squared = b * b;
	float plus4ac = 4.0 * a * c;
	float discriminant = b_squared - plus4ac;
	
	if (discriminant >= 0) { // at least one solution

		t1 = (-b + sqrt(discriminant)) / (2 * a); //because a > 0, t1 > t2 for all values of a,b,c
		t2 = (-b - sqrt(discriminant)) / (2 * a);

		if (t1 > 0) {
			intersections.push_back((ray.basePoint + ray.directionVector * t1));
		}
		if (t2 > 0) {
			intersections.push_back(ray.basePoint + ray.directionVector * t2);
		}

		if (t1 == t2) {
			intersections.pop_back();
		}
	}

	return intersections;


}

Plane::Plane(Color ambient, Color diffuse, Color specular, float distFromOrigin, Vector planeNormal, char t) :
	ObjectShape(ambient, diffuse, specular, t), signedDistanceFromOrigin(distFromOrigin),
	planeNormal(planeNormal) 
	{
	normalizedPlaneNormal = planeNormal; //copy ASSIGNMENT
	normalizedPlaneNormal.NormalizeVec();

	};

std::vector<Point> Plane::intersectionWithRay(Ray ray) {
	std::vector <Point> intersections ;
	Vector startPointVec = ray.basePoint - Point();// S


	//you should also support the case when V*N = 0 i.e the ray is parallel to the plane
	float t = -(this->signedDistanceFromOrigin + startPointVec.operator*(this->planeNormal)) / ray.directionVector.operator*(this->planeNormal); // t=( -D - (S * N) ) / (V * N)

	if (t >= 0) {
		intersections.push_back(ray.basePoint + ray.directionVector * t);
	}

	return intersections;

}

Vector Plane::normalAtPoint(Point p) {
	return normalizedPlaneNormal;
}

Color Plane::getDiffuse(Point interP) {
	int numPatternWidth = 128;
	int numPatternHeight = 128;

	int pixelCoordX = 255 * interP.getX();
	int pixelCoordY = 255 * interP.getY();


	int slot0 = (pixelCoordX / numPatternWidth);
	int slot1 = (pixelCoordY / numPatternHeight);

	//slot0 = (int)(pixelCoordX * 1.5);
	//slot1 = (int)(pixelCoordX * 1.5);
	if (slot0 % 2 != slot1 % 2) { // then "BLACK"
		return Color(0.5, 0.5, 0.5, 0.5);
	}
	
	return diffuse; // otherwise "WHITE" in the checkerboard pattern

}