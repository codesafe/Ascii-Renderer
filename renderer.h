#ifndef RENDERER_H
#define RENDERER_H

#include "predef.h"
#include "raster.h"

class Renderer
{
private:
	Raster raster;

public :
	Renderer();
	~Renderer();

	void Draw_Triangle(Vertex& v0, Vertex& v1, Vertex& v2);
};





#endif