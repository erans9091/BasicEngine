#include "Color.h"


Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};

Color::Color(const Color & otherCol) : r(otherCol.getR()), g(otherCol.getG()), b(otherCol.getB()), a(otherCol.getA()) {};

Color::~Color() {  };

Color & Color::operator=(const Color & otherCol)
{
	r = otherCol.getR();
	g = otherCol.getG();
	b = otherCol.getB();
	a = otherCol.getA();
	return *this;
}

Color Color::operator+ (Color otherCol) {
	return Color(r + otherCol.getR(), g + otherCol.getG(), b + otherCol.getB(), a + otherCol.getA());
}
Color Color::operator* (Color modulator) {
	return Color(r * modulator.getR(), g * modulator.getG(), b * modulator.getB(), a * modulator.getA());
}

Color	Color::Modulate (float ratio) {
	return Color(r * ratio, g * ratio, b * ratio, a * ratio);
}

float Color::getR() const {
	return r;
}

float Color::getG() const {
	return g;
}

float Color::getB() const {
	return b;
}

float Color::getA() const {
	return a;
}

