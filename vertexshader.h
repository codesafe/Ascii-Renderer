#pragma once

#include "predef.h"


class VertexShader
{
public:
	MAT mvp;
	Vec pos;
	Vec normal;
	Vec worldpos;
	UV uv;

public :
	VertexShader(MAT& world, MAT& view, MAT& proj, Vec& pos, Vec& nor, UV& uv);
	~VertexShader();




};