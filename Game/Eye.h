#pragma once

#include "Point.h";

class Eye
{
public:
	Eye();
	Eye(Point p, float bon);
	float getX() const { return p.getX(); };
	float getY() const { return p.getY(); };
	float getZ() const { return p.getZ(); };
	Point getP() const;
private:
	Point p;
	float bonus;
};

