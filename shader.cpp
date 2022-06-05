#include "shader.h"


Shader::Shader()
{
	Identity(&world);
}

Shader::~Shader()
{

}

void Shader::setLight(const Vec& L, const Color& C, float _shine)
{
	Light = L;
	light_color = C;
	shine = _shine;
}

void Shader::setWord(const MAT& mat)
{
	world = mat;
}

void Shader::setView(const Vec& pos, const Vec& at, const Vec& up)
{
	Identity(&view);
	MatrixLookAtLH(&view, pos, at, up);
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
	MAT tm = world * view * projection * viewport;
	Vertex ret;
	ret.pos = vtx.pos;
	ret.normal = world * vtx.normal;
	ret.worldpos = world * ret.pos;
	ret.pos = tm * ret.pos;

	ret.uv = vtx.uv;
	return ret;
}


Color Shader::pixelShader(const float& Ka, const Color& Kd, const Color& Ks)
{
	Color shade, ambient, diffuse, specular;
	ambient = pixel.color * 0.3f;
	//diffuse = Kd * pixel.color * saturate((-Light) * pixel.normal);
	Vec View = Camera - pixel.world_pos;
	Vec H = (View + (-Light)).norm();
	//specular = Ks * pow(saturate(H * pixel.normal), shine);
	return ambient + diffuse + specular;
}


void Shader::setPixel(const Vec& _n, const Vec& _w, const Color& _c)
{
	pixel.normal = _n; 
	pixel.world_pos = _w; 
	pixel.color = _c;
}