#include "renderer.h"

Renderer::Renderer()
{
}
Renderer::~Renderer()
{

}

void Renderer::init()
{
	light.pos = Vec(1, 0, -1);
	light.dir = -light.pos;
	light.dir.norm();
	light.color = Color(1, 1, 1);

	raster.init();
	//model.LoadModel("cube-tex.obj");
	model.LoadModel("b.obj");
	//model.LoadModel("d.obj");

	float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
	float fov = 45.f;

	// Camera
	Vec eye, at, up;

	eye.x = 0;	eye.y = 0.5f;	eye.z = -5;
	at.x = 0;	at.y = 0.5f;	at.z = 1;
	up.x = 0;	up.y = 1;	up.z = 0;

	shader.setView(eye, at, up);
	shader.setProjection(fov * _DEGREE, aspect, MIN_Z, MAX_Z);

// 	Identity(&viewport);
// 	// 0 ~ 1로 하던가 -1 ~ +1
// 	MatrixSetViewPort(&viewport, 0, 0, SCREEN_XSIZE, SCREEN_YSIZE, 0, 1);
	shader.setViewport(0, 0, SCREEN_XSIZE, SCREEN_YSIZE, 0, 1);
	raster.campos = eye;

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
	//r += 0.5f;

	for (int i = 0; i < model.facenum * 3; i+=3)
	{
		int _v1 = model.face[i + 0].vertex;
		int _v2 = model.face[i + 1].vertex;
		int _v3 = model.face[i + 2].vertex;

		int vn1 = model.face[i + 0].normal;
		int vn2 = model.face[i + 1].normal;
		int vn3 = model.face[i + 2].normal;

		int uv1 = model.face[i + 0].uv;
		int uv2 = model.face[i + 1].uv;
		int uv3 = model.face[i + 2].uv;

		Vertex v1, v2, v3;
		v1.pos = model.vertex[_v1];
		v2.pos = model.vertex[_v2];
		v3.pos = model.vertex[_v3];

		v1.normal = rot * model.normal[vn1];
		v2.normal = rot * model.normal[vn2];
		v3.normal = rot * model.normal[vn3];

		v1.uv = model.uv[uv1];
		v2.uv = model.uv[uv2];
		v3.uv = model.uv[uv3];

		raster.drawtriangle(shader, v3, v2, v1);
	}

//	raster.postprocess();
}

unsigned int* Renderer::getscreenbuffer()
{
	return raster.getscreenbuffer();
}

void Renderer::toggle()
{
	raster.togglev = !raster.togglev;
}