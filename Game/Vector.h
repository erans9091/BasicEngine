
#pragma once
class Point;


class Vector {
	public:

		int isNormalized(void);
		void NormalizeVec(void);
		Vector(float x, float y, float z);
		Vector();
		Vector(const Vector& otherVec);
		~Vector();
		Vector & operator=(const Vector& otherVec);
		float operator* ();
		Vector Negate();
		//Vector operator+ (Point p); //return the vector incident at point p with displacment V where v is a vector!
		float operator * (Vector v1); //the dot product
		Vector operator * (float scaleValue);
		Vector operator - (const Vector& otherVec);
		float Norm (void);
		float getX (void) const;
		float getY(void) const;
		float getZ(void) const;
private:
		float x;
		float y;
		float z;

};