#ifndef MODEL_H
#define MODEL_H

#include "predef.h"


class Model
{
public:
	int vertexnum;
	int normalnum;
	int facenum;
	int uvnum;

	Vec* vertex;
	Vec* normal;
	UV *uv;
	Face* face;

	Model();
	~Model();

	bool LoadModel(const char* fname);
};


#endif