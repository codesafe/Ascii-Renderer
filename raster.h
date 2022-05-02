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

	float edge(const Vec3& a, const Vec3& b, const Vec3& c);
	void drawpoint(int x, int y, float z, int color);
	void drawtriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	int readtexel(float u, float v);

	TEXTURE* loadtexture(const char* fname);
};


#endif