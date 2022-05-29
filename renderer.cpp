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

	// Init view mat
	Identity(&view);
	MatrixLookAtLH(&view, eye, at, up);

	// projection mat
	Identity(&proj);
	MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, MIN_Z, MAX_Z);

	Identity(&viewport);
	// 0 ~ 1로 하던가 -1 ~ +1
	MatrixSetViewPort(&viewport, 0, 0, SCREEN_XSIZE, SCREEN_YSIZE, 0, 1);

	raster.campos = eye;

}

void Renderer::clearscreen()
{
	raster.clearscreen();
}

void Renderer::render()
{
	static float r = 0.0f;

	MAT final = view * proj * viewport;

	MAT rot, rot2;
	Identity(&rot);
	Identity(&rot2);
	MatrixRotationY(&rot, r * _DEGREE);
	//MatrixRotationX(&rot2, r * _DEGREE * 1.f);
	rot = rot * rot2;
	r += 0.5f;
	//r = 45;

	for (int i = 0; i < model.facenum * 3; i+=3)
	{
		int _v0 = model.face[i + 0].vertex;
		int _v1 = model.face[i + 1].vertex;
		int _v2 = model.face[i + 2].vertex;

		int vn0 = model.face[i + 0].normal;
		int vn1 = model.face[i + 1].normal;
		int vn2 = model.face[i + 2].normal;

		int uv0 = model.face[i + 0].uv;
		int uv1 = model.face[i + 1].uv;
		int uv2 = model.face[i + 2].uv;

		VertexShader v0, v1, v2;

		v0.uv = model.uv[uv0];
		v1.uv = model.uv[uv1];
		v2.uv = model.uv[uv2];

		v0.normal = rot * model.normal[vn0];
		v1.normal = rot * model.normal[vn1];
		v2.normal = rot * model.normal[vn2];

		v0.worldpos = rot * model.vertex[_v0];
		v1.worldpos = rot * model.vertex[_v1];
		v2.worldpos = rot * model.vertex[_v2];

		Vec temp;
		Transform(&temp, v0.worldpos, final);
		PerspectiveDivide(&v0.pos, temp);

		Transform(&temp, v1.worldpos, final);
		PerspectiveDivide(&v1.pos, temp);

		Transform(&temp, v2.worldpos, final);
		PerspectiveDivide(&v2.pos, temp);

		raster.drawtriangle(light, v2, v1, v0);
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