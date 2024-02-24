#pragma once

#include <vector>
#include <string>
#include "Point.h"
#include "Eye.h"

class Light;
class ObjectShape;
class Eye;
class Color;
class Ray;

typedef std::string str;

struct intersectionData
{
	Point interP;
	int shapeIntersectedIdx;

};


class RayTracer {
public:
	RayTracer(std::string path);
	void RayTrace();
	Color RecursiveRayTrace(Ray incomingRay , int recursionLevel, int isInitialRay);
	intersectionData * SceneIntersectionWithRay(Ray ray,  int initial);
	unsigned char* getTextureData();
	int InShadow(Ray intersectionRay , int intersectedShapeIdx);
	Ray getTranspRay(Ray r,Point interPnt,int shapeIdx);
private:
	std::vector<ObjectShape*>objects;
	std::vector<Light* > sceneLights;

	Point topLeft;
	Point bottomRight;

	unsigned char* texture_data;
	int width;
	int height;
	float xDelta;
	float yDelta;

	Eye cam;
	static std::vector<float> popIntens(std::vector<str>& v, int idx);
	static std::vector<float> popSpot(std::vector<str>& v, int idx);
	static Color popColor(std::vector<str>& v, int idx);
	static std::vector<float> popByLtr(std::vector<str>& v, int idx, char l);
};