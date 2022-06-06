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
	vertex = new Vec[vertexnum];
	for (i = 0; i < vertexnum; i++)
		ret = fscanf(fp, "%s %f %f %f", dummy, &vertex[i].x, &vertex[i].y, &vertex[i].z);

	// uv
	ret = fscanf(fp, "%d", &uvnum);
	uv = new UV[uvnum];
	for (i = 0; i < uvnum; i++)
		ret = fscanf(fp, "%s %f %f", dummy, &uv[i].u, &uv[i].v);

	// normal
	ret = fscanf(fp, "%d", &normalnum);
	normal = new Vec[normalnum];
	for (i = 0; i < normalnum; i++)
		ret = fscanf(fp, "%s %f %f %f", dummy, &normal[i].x, &normal[i].y, &normal[i].z);

	ret = fscanf(fp, "%d", &facenum);
	face = new Face[facenum * 3];
	for (i = 0; i < facenum * 3; i += 3)
	{
		int v0, v1, v2;
		int uv0, uv1, uv2;
		int n0, n1, n2;
		fscanf(fp, "%s %d/%d/%d %d/%d/%d %d/%d/%d",
			dummy, 
			&v0, &uv0, &n0, &v1, &uv1, &n1, &v2, &uv2, &n2 );

		face[i + 0].vertex = v0 - 1;
		face[i + 1].vertex = v1 - 1;
		face[i + 2].vertex = v2 - 1;

		face[i + 0].uv = uv0 - 1;
		face[i + 1].uv = uv1 - 1;
		face[i + 2].uv = uv2 - 1;

		face[i + 0].normal = n0 - 1;
		face[i + 1].normal = n1 - 1;
		face[i + 2].normal = n2 - 1;
	}


	fclose(fp);
	return true;
}

Vec Model::GetVertex(int idx)
{
	int v = face[idx].vertex;
	return vertex[v];
}

Vec Model::GetNormal(int idx)
{
	int v = face[idx].normal;
	return normal[v];
}

UV Model::GetUV(int idx)
{
	int v = face[idx].uv;
	return uv[v];
}