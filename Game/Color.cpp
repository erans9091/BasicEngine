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

void Color::ClampColor() {
	clampR();
	clampG();
	clampB();
}

void Color::clampR() {
	if (r < 0.0) {
		r = 0;
	}
	else if (r > 1.0) {
		r = 1;
	}
}


void Color::clampG() {
	if (g < 0.0) {
		g = 0;
	}
	else if (g > 1.0) {
		g = 1;
	}
}

void Color::clampB() {
	if (b < 0.0) {
		b = 0;
	}
	else if (b > 1.0) {
		b = 1;
	}
}