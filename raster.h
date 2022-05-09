#ifndef RASTER_H
#define RASTER_H

#include "predef.h"

class Raster
{
private:
	unsigned int* screenbuffer;
	float* zbuffer;
	TEXTURE* texture;

public:
	Raster();
	~Raster();

	void init();
	void clearscreen();
	unsigned int* getscreenbuffer();

	float edge(const Vec4& a, const Vec4& b, const Vec4& c);
	void drawpoint(int x, int y, float z, int color);
	void drawtriangle(Vertex& v1, Vertex& v2, Vertex& v3, float bright);
	int readtexel(float u, float v);

	TEXTURE* loadtexture(const char* fname);
};


#endif