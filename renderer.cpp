#include "renderer.h"

#define USE_DRAGON


Renderer::Renderer()
{
}
Renderer::~Renderer()
{

}

void Renderer::init()
{
	raster.init();

#ifdef USE_DRAGON
	model.LoadModel("d.obj");
#else
	model.LoadModel("cube-tex.obj");
	//model.LoadModel("b.obj");
#endif

	float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
	float fov = 45.f;

	// Camera
#ifdef USE_DRAGON
	Vec eye(0, 0.5f, -20);
	Vec at(0, 0.5f, 1);
	Vec up(0, 1, 0);
#else
	Vec eye(0, 0.5f, -5);
	Vec at(0, 0.5f, 1);
	Vec up(0, 1, 0);
#endif
	shader.setView(eye, at, up);
	shader.setProjection(fov * _DEGREE, aspect, MIN_Z, MAX_Z);
	shader.setViewport(0, 0, SCREEN_XSIZE, SCREEN_YSIZE, 0, 1);

	Vec lpos = Vec(4, -3, -3.0f);
#ifdef USE_DRAGON
	shader.setLight(lpos, Vec(0,0,0), Color(1, 0.5f, 0.4f), 512);
#else
	shader.setLight(lpos, Vec(0, 0, 0), Color(1, 1, 1), 20);
#endif
}

void Renderer::clearscreen()
{
	raster.clearscreen();
}

void Renderer::render()
{
	static float r = 0.0f;

	MAT rot;
	Identity(&rot);
	MatrixRotationY(&rot, r * _DEGREE);
	r += 0.5f;
	shader.setWorld(rot);

// 	Vec lpos = Vec(4, -3, -3.0f);
// 	lpos = rot * lpos;
// 	shader.setLight(lpos, Vec(0, 0, 0), Color(1, 1, 1), 20);
// 	r += 1;


	for (int i = 0; i < model.facenum * 3; i+=3)
	{
		Vertex v1, v2, v3;
 		v1.pos = model.GetVertex(i + 0);
 		v2.pos = model.GetVertex(i + 1);
 		v3.pos = model.GetVertex(i + 2);

		v1.normal = model.GetNormal(i + 0);
		v2.normal = model.GetNormal(i + 1);
		v3.normal = model.GetNormal(i + 2);

		v1.uv = model.GetUV(i + 0);
		v2.uv = model.GetUV(i + 1);
		v3.uv = model.GetUV(i + 2);

		raster.drawtriangle(shader, v3, v2, v1);
	}

	//raster.postprocess();
}

unsigned int* Renderer::getscreenbuffer()
{
	return raster.getscreenbuffer();
}

void Renderer::toggle()
{
	raster.togglev = !raster.togglev;
}