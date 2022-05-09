#include "renderer.h"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

void Renderer::init()
{
	light.pos = Vec3(0, 1, 0);
	light.dir = Vec3(0, 0.5f, 0.5f);
	Normalize(&light.dir);


	raster.init();
	model.LoadModel("cube-tex.obj");
	//model.LoadModel("b.obj");
	//model.LoadModel("d.obj");

	float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
	float fov = 45.f;

	// Camera
	Vec3 eye;
	Vec3 at;
	Vec3 up;

	eye.x = 0;	eye.y = 0;	eye.z = -15;
	at.x = 0;	at.y = 0;	at.z = 1;
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
		int _v0 = model.face[i + 0].vertex - 1;
		int _v1 = model.face[i + 1].vertex - 1;
		int _v2 = model.face[i + 2].vertex - 1;

		int uv0 = model.face[i + 0].uv - 1;
		int uv1 = model.face[i + 1].uv - 1;
		int uv2 = model.face[i + 2].uv - 1;

		Vertex v0, v1, v2;

		v0.uv = model.uv[uv0];
		v1.uv = model.uv[uv1];
		v2.uv = model.uv[uv2];

		Vec4 temp;
		Vec3 t1 = rot * model.vertex[_v0];
		Transform4(&temp, t1, final);
		PerspectiveDivide(&v0.pos, temp);

		Vec3 t2 = rot * model.vertex[_v1];
		Transform4(&temp, t2, final);
		PerspectiveDivide(&v1.pos, temp);

		Vec3 t3 = rot * model.vertex[_v2];
		Transform4(&temp, t3, final);
		PerspectiveDivide(&v2.pos, temp);

		float bright = 1;

		//raster.drawtriangle(v0, v1, v2);
		raster.drawtriangle(v2, v1, v0, bright);
	}
	
}

unsigned int* Renderer::getscreenbuffer()
{
	return raster.getscreenbuffer();
}

