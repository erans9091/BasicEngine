#include "Eye.h"
#include "Point.h"

Eye::Eye() : p(0, 0, 0)
{

	bonus = 0;
}

Eye::Eye(Point p, float bon) :p(p)
{
	bonus = bon;
}
Point Eye::getP() const {
	return this->p;
}