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
// 	FILE* fp = fopen(fname, "rt");
// 	int i;
// 	if (fp == NULL)
// 		return false;
// 
// 	fscanf(fp, "%d", &vertexnum);
// 
// 	vertex = new Vertex[vertexnum];
// 	for (i = 0; i < vertexnum; i++)
// 	{
// 		vertex[i].diffuse = 0xFFFFFFFF;
// 
// 		fscanf(fp, "%f %f %f %f %f %f %f %f",
// 			&vertex[i].v.x, &vertex[i].v.y, &vertex[i].v.z,
// 			&vertex[i].uv.u, &vertex[i].uv.v,
// 			&vertex[i].normal.x, &vertex[i].normal.y, &vertex[i].normal.z);
// 	}
// 
// 	fscanf(fp, "%d", &facenum);
// 	face = new unsigned short[facenum * 3];
// 	for (i = 0; i < facenum * 3; i += 3)
// 		fscanf(fp, "%d %d %d", &face[i + 0], &face[i + 1], &face[i + 2]);
// 
// 	fclose(fp);
	return true;
}
