#ifndef PREDEF_H
#define PREDEF_H

#pragma warning(disable: 4996)

#include "math.h"

#define AppName "SOFTWARE-RENDERER"

#define	_PI			3.141592f
#define _DEGREE		(3.141592f / 180.0f)

#define SCREEN_XSIZE	640
#define SCREEN_YSIZE	320
#define MIN_Z		3
#define MAX_Z		100

#define ASCII_XSIZE		160
#define ASCII_YSIZE		40

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define UNPACK_R(C) (int)(( (C) >> 24 ) & 0xFF )
#define UNPACK_G(C) (int)(( (C) >> 16 ) & 0xFF )
#define UNPACK_B(C) (int)(( (C) >> 8 ) & 0xFF )

#define PACK_RGB(r,g,b)	(r << 24 | g << 16 | b << 8 | 0xff)


//#define DRAW_DEPTH
//#define ASCII_RENDER
//#define ASCII_RENDER_ONLY

struct TEXTURE
{
	int w, h, pitch;
	unsigned char* image;
};

struct UV
{
	float u, v;
};

static float saturate(float x)
{
	return fminf(1.0f, fmaxf(0.0f, x));
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
		return R << 24 | G << 16 | B << 8 | 0xff;
	}
};

struct Vertex
{
	Vec pos;
	Vec normal;
	Vec worldpos;
	UV uv;
	float rhw;
};

struct Face
{
	int vertex;
	int uv;
	int normal;
};

struct Light
{
	Vec pos;
	Vec dir;
	Color color;
};


#endif