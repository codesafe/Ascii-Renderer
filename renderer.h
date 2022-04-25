#ifndef RENDERER_H
#define RENDERER_H

#include "math.h"

struct UV
{ 
	float u, v; 
};

struct Vertex
{
	Vec3 pos; 
	UV uv; 
	//int color; 
	//float rhw;
};


void Draw_Triangle(Vertex &v0, Vertex& v1, Vertex& v2);


#endif