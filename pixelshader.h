#pragma once

#include "predef.h"
#include "math.h"


class PixelShader
{
public:
	MAT viewport;
	Vec normal;
	Vec world_pos;
	Color color;
	
	Vec lightpos;
	Vec camerapos;

public :
	PixelShader(const Vec& _n, const Vec& _w, const Color& _c);
	~PixelShader();

	void setViewport(MAT& vp);
	void setlight(Vec& _light, Vec& _camera);
	Color getpixelColor(const float& Ka, const Color& Kd, const Color& Ks);
};