#include "predef.h"
#include "model.h"

Model::Model()
{
	vertexnum = 0;
	facenum = 0;
	vertex = nullptr;
	face = nullptr;
}

Model::~Model()
{
	delete[] vertex;
	delete[] face;
}


bool Model::LoadModel(const char* fname)
{
	char dummy[4];
	FILE* fp = fopen(fname, "rt");
	int i;
	if (fp == NULL)
		return false;

	// vertex
	int ret = fscanf(fp, "%d", &vertexnum);
	vertex = new Vec3[vertexnum];
	for (i = 0; i < vertexnum; i++)
		ret = fscanf(fp, "%s %f %f %f", dummy, &vertex[i].x, &vertex[i].y, &vertex[i].z);

	// uv
	ret = fscanf(fp, "%d", &uvnum);
	uv = new UV[uvnum];
	for (i = 0; i < uvnum; i++)
		ret = fscanf(fp, "%s %f %f", dummy, &uv[i].u, &uv[i].v);

	// normal
	ret = fscanf(fp, "%d", &normalnum);
	normal = new Vec3[normalnum];
	for (i = 0; i < normalnum; i++)
		ret = fscanf(fp, "%s %f %f %f", dummy, &normal[i].x, &normal[i].y, &normal[i].z);

	ret = fscanf(fp, "%d", &facenum);
	face = new Face[facenum * 3];
	for (i = 0; i < facenum * 3; i += 3)
		fscanf(fp, "%s %d/%d/%d %d/%d/%d %d/%d/%d", 
			dummy,
			&face[i + 0].vertex, &face[i + 0].uv, &face[i + 0].normal,
			&face[i + 1].vertex, &face[i + 1].uv, &face[i + 1].normal,
			&face[i + 2].vertex, &face[i + 2].uv, &face[i + 2].normal);

	fclose(fp);
	return true;
}
