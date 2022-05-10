#ifndef PREDEF_H
#define PREDEF_H

#pragma warning(disable: 4996)

#include "math.h"

#define AppName "SOFTWARE-RENDERER"

#define	_PI			3.141592f
#define _DEGREE		(3.141592f / 180.0f)

#define SCREEN_XSIZE	640
#define SCREEN_YSIZE	360
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

struct Vertex
{
	Vec4 pos;
	Vec3 normal;
	UV uv;
	float rhw;
};

struct Face
{
	int vertex;
	int uv;
	int normal;
};


#endif