#pragma once

#include "vertexshader.h"

VertexShader::VertexShader(MAT& world, MAT& view, MAT& proj, Vertex& v)
{
	mvp = world * view * proj;

	Transform(&worldpos, v.pos, world);
	Transform(&normal, v.normal, world);
	Transform(&pos, v.pos, mvp);
	uv = v.uv;
}

VertexShader::~VertexShader()
{
}


