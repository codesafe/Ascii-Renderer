#ifndef PREDEF_H
#define PREDEF_H

#pragma warning(disable: 4996)

#include "math.h"

#define AppName "SOFTWARE-RENDERER"
#define SCREEN_XSIZE	640
#define SCREEN_YSIZE	480
#define MIN_Z		0.1f
#define MAX_Z		10

#define	_PI			3.141592f
#define _DEGREE		(3.141592f / 180.0f)

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


struct TEXTURE
{
	int w, h, pitch;
	unsigned char* image;
};

struct UV
{
	float u, v;
};

struct Vertex
{
	Vec3 pos;
	UV uv;
};

struct Face
{
	int vertex;
	int uv;
	int normal;
};


#endif