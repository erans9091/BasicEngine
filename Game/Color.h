#pragma once
class Color {
public:

	~Color();
	Color(const Color& otherCol);
	Color(float r, float g, float b, float a);
	Color Modulate (float ratio);
	Color& operator= (const Color& otherCol);
	Color operator* (Color modulator);
	Color operator+ (Color otherCol);
	float getR() const;
	float getG() const;
	float getB() const;
	float getA() const;

private:

	float r;
	float g;
	float b;
	float a;
};