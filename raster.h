#ifndef RASTER_H
#define RASTER_H

#include "predef.h"
#include "color.h"
#include "shader.h"

class Raster
{
private:
	unsigned int* screenbuffer;
	float* zbuffer;
	TEXTURE* texture;


public:
	bool togglev;

	Raster();
	~Raster();

	void init();
	void clearscreen();
	unsigned int* getscreenbuffer();

	int readtexel(float u, float v);
	int gettexturepixel(int x, int y);

	float edge(const Vec& a, const Vec& b, const Vec& c);

	void drawpoint(int x, int y, float z, int color);
	void drawline(float x1, float y1, float x2, float y2, int color);
	void drawTriangleOutline(Vertex v0, Vertex v1, Vertex v2, int color);
	void drawtriangle(Shader &shader, Vertex& v1, Vertex& v2, Vertex& v3);

	int getpoint(int x, int y);
	void postprocess();

	Vec getfacenormal(Vec& v0, Vec& v1, Vec& v2);

	TEXTURE* loadtexture(const char* fname);
};


#endif