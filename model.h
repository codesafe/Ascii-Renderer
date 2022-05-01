#ifndef MODEL_H
#define MODEL_H

#include "predef.h"
#include "renderer.h"


class Model
{
public:
	int vertexnum;
	int normalnum;
	int facenum;
	int uvnum;

	Vec3* vertex;
	Vec3* normal;
	UV *uv;
	Face* face;

	Model();
	~Model();

	bool LoadModel(const char* fname);
};


#endif