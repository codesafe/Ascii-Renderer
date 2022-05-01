#ifndef MODEL_H
#define MODEL_H

#include "math.h"

struct UV
{ 
	float u, v; 
};

struct Vertex
{
	Vec3 v; 
	Vec3 normal;
	unsigned long diffuse;
	UV uv; 
};

struct Face
{
	int vertex;
	int uv;
	int normal;
};


class Model
{
public:
	int vertexnum;
	int facenum;
	Vertex* vertex;
	unsigned short* face;

	Model();
	~Model();

	bool LoadModel(const char* fname);
};


#endif