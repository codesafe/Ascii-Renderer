#pragma once

//#include "predef.h"
#include <cmath>

static float saturate(float x)
{
	return fmin(1.0f, fmax(0.0f, x));
}

struct Color 
{
	float r, g, b, a;
	Color() :r(0.f), g(0.f), b(0.f), a(0.f) {}
	Color(float _r, float _g, float _b, float _a = 1.0f) :r(_r), g(_g), b(_b), a(_a) {}
	Color(int color)
	{
		r = (float)UNPACK_R(color) / 255.f;
		g = (float)UNPACK_G(color) / 255.f;
		b = (float)UNPACK_B(color) / 255.f;
		a = 1;
	}

	Color operator +(const Color& x) const 
	{
		float _r = r + x.r > 1 ? 1 : r + x.r;
		float _g = g + x.g > 1 ? 1 : g + x.g;
		float _b = b + x.b > 1 ? 1 : b + x.b;

		return Color(_r, _g, _b);
	}

	Color operator *(const float x) const 
	{
		return Color(x * r, x * g, x * b, x * a);
	}

	Color operator *(const Color& w) const 
	{
		return Color(r * w.r, g * w.g, b * w.b, a * w.a);
	}

	operator unsigned int()
	{
		unsigned int R = (unsigned int)round(saturate(r) * 255);
		unsigned int G = (unsigned int)round(saturate(g) * 255);
		unsigned int B = (unsigned int)round(saturate(b) * 255);
		return R << 24 | G << 16| B << 8 | 0xff;
	}
};