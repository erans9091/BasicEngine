
#pragma once
class Vector;

class Point {
public:
	Vector operator- (const Point& p);
	Point();
	Point(float x, float y, float z);
	Point(const Point& p);
	~Point();
	Point operator+ (const Vector& v);
	float getX() const;
	float getY() const;
	float getZ() const;
private:
	float x;
	float y;
	float z;
};
/*

class Point {
public:
	float x;
	float y;
	float z;

	Vector operator- (const Point & p );
	Point();
	Point(const Point& p);
	~Point();
	Point operator +(const Vector & v);
	Point(float x, float y, float z);
};
*/