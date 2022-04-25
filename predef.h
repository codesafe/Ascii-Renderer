#ifndef PREDEF_H
#define PREDEF_H

#pragma warning(disable: 4996)

#define AppName "SOFTWARE-RENDERER"
#define SCREEN_XSIZE	640
#define SCREEN_YSIZE	480


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



#endif