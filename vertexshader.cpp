#pragma once

#include "vertexshader.h"

VertexShader::VertexShader(MAT& world, MAT& view, MAT& proj, Vec& pos, Vec& nor, UV &uv)
{
	mvp = world * view * proj;

	worldpos = world * pos;
	normal = world * nor;
	this->pos = mvp * pos;

	PerspectiveDivide(&this->pos, this->pos);

	//Transform(&worldpos, pos, world);
	//Transform(&normal, nor, world);
	//Transform(&pos, pos, mvp);
	this->uv = uv;
}

VertexShader::~VertexShader()
{
}


