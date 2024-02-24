#include "ObjectShape.h"
#include "Color.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

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

		if (t2 > 0.01) {
			intersections.push_back((ray.basePoint + ray.directionVector * t2));
		}
		if (t1 > 0.01) {
			intersections.push_back(ray.basePoint + ray.directionVector * t1);
		}

		if (t1 == t2 && intersections.size() > 0) {
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

std::vector<Point> Plane::intersectionWithRay(Ray ray) { // ray: S+ t*V
	std::vector <Point> intersections ;
	Vector startPointVec = ray.basePoint - Point();// S


	//you should also support the case when V*N = 0 i.e the ray is parallel to the plane
	float dotRayDirAndNormalVec = this->planeNormal * ray.directionVector;
	if (dotRayDirAndNormalVec == 0) { //V*N = 0 i.e the ray is parallel to the plane
		std::cout << "this ray is parallel to the plane!" << std::endl;
		//check if the base point is on the plane
		if (startPointVec * this->planeNormal + signedDistanceFromOrigin == 0) { //i.e S*N + D =0, base point on the plane!
//			std::cout << "this ray is parallel to the plane!" << std::endl;
	//		intersections.push_back(Point(ray.basePoint)); 
		}
	}
	else {
		float t = -(this->signedDistanceFromOrigin + ( startPointVec * planeNormal) ) / (ray.directionVector * planeNormal); // t=( -D - (S * N) ) / (V * N)
		Point intersectionPoint = ray.basePoint + ray.directionVector * t;
		if (t > 0.01 && intersectionPoint.getZ() < 0.0) {
			intersections.push_back(intersectionPoint);
		}
	}


	return intersections;

}

Vector Plane::normalAtPoint(Point p) {
	return normalizedPlaneNormal.Negate();
}

Color Plane::getDiffuse(Point interP) {
	float scaleParameter = 0.5f;
	float chessboard = 0;

	float pixelCoordX = interP.getX();
	float pixelCoordY =   interP.getY();

	if(pixelCoordX < 0) {
		chessboard += floor((0.5 - pixelCoordX) / scaleParameter);
	}
	else {
		chessboard += floor(pixelCoordX / scaleParameter);
	}

	if(pixelCoordY < 0) {
		chessboard += floor((0.5 - pixelCoordY) / scaleParameter);
	}
	else {
		chessboard += floor(pixelCoordY / scaleParameter);
	}

	chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
	chessboard *= 2;

	if (chessboard > 0.5) {
		return diffuse.Modulate(0.5);
	}

	return diffuse;


}