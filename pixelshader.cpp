#include "pixelshader.h"


PixelShader::PixelShader(const Vec& _n, const Vec& _w, const Color& _c)
{
	normal = _n;
	world_pos = _w;
	color = _c;
}

PixelShader::~PixelShader()
{

}

void PixelShader::setViewport(MAT& vp)
{
	viewport = vp;
}

void PixelShader::setlight(Vec& _light, Vec& _camera)
{
	lightpos = _light;
	camerapos = _camera;
}

Color PixelShader::getpixelColor(const float& Ka, const Color& Kd, const Color& Ks)
{
	Color shade, ambient, diffuse, specular;
	ambient = color * 0.3f;
	diffuse = Kd * color * saturate(-lightpos.dot(normal));
	Vec View = camerapos - world_pos;
	Vec H = (View - lightpos);
	H.norm();

	float shine = 1;
	specular = Ks * powf( saturate( H.dot(normal) ), shine);
	return ambient + diffuse + specular;
}