#pragma once

#include "predef.h"

struct Pixel
{
public:
	Vec normal;
	Vec world_pos;
	Color color;
};


class Shader
{
public:

	MAT world;
	MAT view;
	MAT projection;
	MAT viewport;

	MAT resultTM;

	Light light;

	Vec camerapos;
	Pixel pixel;

	Color specolor;
	float shine;

public :
	Shader();
	~Shader();

	void setLight(const Vec& pos, const Vec& at, const Color& C, float _shine);

	void setWorld(const MAT& mat);
	void setView(const Vec& pos, const Vec& at, const Vec& up);
	void setProjection(float fovy, float aspect, float n, float f);
	void setViewport(float x, float y, float w, float h, float minz, float maxz);

	Vec wvp(const Vec& vec);
	Vertex wvp(const Vertex& vtx);

	Vertex vertexShader(const Vertex& vtx);

	Color pixelShader(const float& Ka, const Color& Kd, const Color& Ks);
	void setPixel(const Vec& _n, const Vec& _w, const Color& _c);
};