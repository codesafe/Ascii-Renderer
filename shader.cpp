#include "shader.h"


Shader::Shader()
{
	Identity(&world);
	shine = 1;
}

Shader::~Shader()
{

}

void Shader::setLight(const Vec& pos, const Vec& at, const Color& C, float _shine)
{
	light.pos = pos;
	light.dir = (at-pos).norm();
	light.color = C;
	shine = _shine;
}

void Shader::setWorld(const MAT& mat)
{
	world = mat;
}

void Shader::setView(const Vec& pos, const Vec& at, const Vec& up)
{
	Identity(&view);
	MatrixLookAtLH(&view, pos, at, up);
	camerapos = pos;
}

void Shader::setProjection(float fov, float aspect, float n, float f)
{
	Identity(&projection);
	MatrixPerspectiveFovLH(&projection, fov, aspect, n, f);
}

void Shader::setViewport(float x, float y, float w, float h, float minz, float maxz)
{
	Identity(&viewport);
 	// 0 ~ 1로 하던가 -1 ~ +1
	MatrixSetViewPort(&viewport, 0, 0, SCREEN_XSIZE, SCREEN_YSIZE, 0, 1);

	resultTM = view * projection * viewport;
}

Vec Shader::wvp(const Vec& vec) 
{
	Vec ret;
	ret = world * vec;
	ret = view * ret;
	ret = projection * ret;
	return ret;
}

Vertex Shader::wvp(const Vertex& vtx)
{
	Vertex ret;
	ret.pos = world * vtx.pos;
	ret.normal = world * vtx.normal;
	ret.worldpos = ret.pos;
	ret.pos = view * ret.pos;
	ret.pos = projection * ret.pos;

	return ret;
}

Vertex Shader::vertexShader(const Vertex& vtx)
{
	MAT tm = world * resultTM;
	Vertex ret;

	ret.pos = tm * vtx.pos;
	ret.normal = world * vtx.normal;
	ret.normal.norm();

	ret.worldpos = world * vtx.pos;
	ret.uv = vtx.uv;

	return ret;
}

Vec reflect(Vec& I, Vec& N)
{
	return I - ( N * (2.0f * I.dot(N)) );
}


Color Shader::pixelShader(const float& Ka, const Color& Kd, const Color& Ks)
{
	Color ambient, diffuse, specular;
	ambient = pixel.color * Ka;

	// lambert 
	diffuse = Kd * pixel.color * saturate(-(light.dir.dot(pixel.normal)));

	Vec view = (camerapos - pixel.world_pos).norm();
	Vec h = (view - light.dir).norm();

	//specular = Ks * powf( saturate( h.dot(pixel.normal) ), shine);

	Vec ref = reflect(light.dir, pixel.normal);
	ref.norm();
	specular = Ks * powf(saturate(ref.dot(view)), shine);

	return ambient + diffuse + specular;
}


void Shader::setPixel(const Vec& _n, const Vec& _w, const Color& _c)
{
	pixel.normal = _n; 
	pixel.world_pos = _w; 
	pixel.color = _c;
}