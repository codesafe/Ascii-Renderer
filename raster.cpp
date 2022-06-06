#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <algorithm>
#include <cmath>

#include "raster.h"
#include "color.h"

//#define WIRE_FRAME
#define PERSPETIVE_CORRECT
#define BILINEAR_TEXTURE
#define LIGHTING


Raster::Raster()
{
	togglev = false;
	screenbuffer = nullptr;
	zbuffer = nullptr;
}

Raster::~Raster()
{
	delete[] screenbuffer;
}

void Raster::init()
{
	screenbuffer = new unsigned int[SCREEN_XSIZE * SCREEN_YSIZE * 4];
	zbuffer = new float[SCREEN_XSIZE * SCREEN_YSIZE];
	//texture = loadtexture("texture.bmp");
	texture = loadtexture("gold.bmp");
}

void Raster::clearscreen()
{
	memset((char*)screenbuffer, 0, sizeof(int) * SCREEN_XSIZE * SCREEN_YSIZE);

	for (int i = 0; i < SCREEN_XSIZE * SCREEN_YSIZE; i++)
		zbuffer[i] = 1.0f; // -1 ~ +1
}

unsigned int* Raster::getscreenbuffer()
{
	return screenbuffer;
}

void Raster::drawpoint(int x, int y, float z, int color)
{
	if (x > SCREEN_XSIZE || x < 0) return;
	if (y > SCREEN_YSIZE || y < 0) return;

#ifndef DRAW_DEPTH

	if (zbuffer[x + y * SCREEN_XSIZE] >= z)
	{
		z = z > 1 ? 1 : z;
		z = z < 0 ? 0 : z;
		zbuffer[x + y * SCREEN_XSIZE] = z;
		screenbuffer[(int)x + (int)y * SCREEN_XSIZE] = color;

	}
#else

	if (zbuffer[x + y * SCREEN_XSIZE] >= z)
	{
		//screenbuffer[(int)x + (int)y * SCREEN_XSIZE] = color;
		zbuffer[x + y * SCREEN_XSIZE] = z;

		z = z > 1 ? 1 : z;
		z = z < 0 ? 0 : z;

		int depth_color = (int)lerp(255, 0, z);
		color = depth_color << 24 | depth_color << 16 | depth_color << 8 | 0xff;
		screenbuffer[(int)x + (int)y * SCREEN_XSIZE] = color;
	}

#endif
}

void Raster::drawline(float x1, float y1, float x2, float y2, int color) 
{
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep) 
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	float dx = x2 - x1;
	float dy = fabsf(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x < maxX; x++) 
	{
		if (steep) 
		{
			drawpoint(y, x, 0, color);
		}
		else {
			drawpoint(x, y, 0, color);
		}

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void Raster::drawTriangleOutline(Vertex v0, Vertex v1, Vertex v2, int color) 
{
	float w = min(v0.pos.w, min(v1.pos.w, v2.pos.w));

	if (w < 0)
		return;

	drawline(v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, color);
	drawline(v0.pos.x, v0.pos.y, v2.pos.x, v2.pos.y, color);
	drawline(v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, color);
}

float Raster::edge(const Vec& a, const Vec& b, const Vec& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

Vec Raster::getfacenormal(Vec& v0, Vec& v1, Vec& v2)
{
	// Calculate triangle normal for backface culling
	Vec s1 = v0 - v2;
	Vec s2 = v0 - v1;

	Vec facenormal;
	Cross(&facenormal, s1, s2);
	facenormal.norm();
	return facenormal;
}


void Raster::drawtriangle(Shader& shader, Vertex& _v1, Vertex& _v2, Vertex& _v3)
{
	Vertex v1 = shader.vertexShader(_v1);
	Vertex v2 = shader.vertexShader(_v2);
	Vertex v3 = shader.vertexShader(_v3);

	Vec p1 = v1.pos;
	Vec p2 = v2.pos;
	Vec p3 = v3.pos;

	PerspectiveDivide(&p1, p1);
	PerspectiveDivide(&p2, p2);
	PerspectiveDivide(&p3, p3);

	// back face cull
	Vec facenormal = getfacenormal(p1, p2, p3);
	Vec t = Vec(0, 0, 1.0f);
	if (facenormal.dot(t) < 0)
		return;

	float y1 = p1.y;
	float y2 = p2.y;
	float y3 = p3.y;

	float x1 = p1.x;
	float x2 = p2.x;
	float x3 = p3.x;

	int minx = (int)ceil(min(min(x1, x2) - 0.5f, x3) - 0.5f);
	int maxx = (int)ceil(max(max(x1, x2) + 0.5f, x3) + 0.5f);
	int miny = (int)ceil(min(min(y1, y2) - 0.5f, y3) - 0.5f);
	int maxy = (int)ceil(max(max(y1, y2) + 0.5f, y3) + 0.5f);

	float minu = min(min(v1.uv.u, v2.uv.u), v3.uv.u);
	float maxu = max(max(v1.uv.u, v2.uv.u), v3.uv.u);
	float minv = min(min(v1.uv.v, v2.uv.v), v3.uv.v);
	float maxv = max(max(v1.uv.v, v2.uv.v), v3.uv.v);

	float area = edge(p1, p2, p3);

	minx = minx < 0 ? 0 : minx;
	miny = miny < 0 ? 0 : miny;
	maxx = maxx > SCREEN_XSIZE ? SCREEN_XSIZE : maxx;
	maxy = maxy > SCREEN_YSIZE ? SCREEN_YSIZE : maxy;

	v1.uv.u = v1.uv.u / p1.w;
	v1.uv.v = v1.uv.v / p1.w;

	v2.uv.u = v2.uv.u / p2.w;
	v2.uv.v = v2.uv.v / p2.w;

	v3.uv.u = v3.uv.u / p3.w;
	v3.uv.v = v3.uv.v / p3.w;

	float rhw1 = 1.0f / p1.w;
	float rhw2 = 1.0f / p2.w;
	float rhw3 = 1.0f / p3.w;

	for (int y = miny; y < maxy; y++)
	{
		for (int x = minx; x < maxx; x++)
		{
			Vec p((float)x, (float)y, 0, 1);
			float w0 = edge(p2, p3, p);
			float w1 = edge(p3, p1, p);
			float w2 = edge(p1, p2, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;

				float w = 1.0f / (rhw1 * w0 + rhw2 * w1 + rhw3 * w2);

				float u = v1.uv.u * w0 + v2.uv.u * w1 + v3.uv.u * w2;
				float v = v1.uv.v * w0 + v2.uv.v * w1 + v3.uv.v * w2;
				float z = p1.z * w0 + p2.z * w1 + p3.z * w2;

				u *= w;
				v *= w;

				int color = readtexel(u, v);
				Vec N = (v1.normal * rhw1 * w0 + v2.normal * rhw2 * w1 + v3.normal * rhw3 * w2) * w;
				N.norm();
				Vec W = (v1.worldpos * rhw1 * w0 + v2.worldpos * rhw2 * w1 + v3.worldpos * rhw3 * w2) * w;
				//W.norm();

				shader.setPixel(N, W, color);

				float Ka = 0.3f;
				Color Kd = shader.light.color;
				Color Ks = shader.light.color;
				color = shader.pixelShader(Ka, Kd, Ks);

				drawpoint(x, y, z, color);
			}
		}
	}

}

int Raster::gettexturepixel(int x, int y)
{
	int color = 0xffffffff;

	x = x < 0 ? 0 : x;
	x = x > texture->w ? texture->w : x;
	y = y < 0 ? 0 : y;
	y = y > texture->h ? texture->h : y;

	int b = texture->image[y * texture->pitch + (x * 3 + 0)];
	int g = texture->image[y * texture->pitch + (x * 3 + 1)];
	int r = texture->image[y * texture->pitch + (x * 3 + 2)];

	color = PACK_RGB(r, g, b);
	return color;
}

int Raster::readtexel(float u, float v)
{
#if 1

#ifdef BILINEAR_TEXTURE
	int x = (int)(u * texture->w);
	int y = (int)(v * texture->h);

	if (x >= texture->w) x = texture->w - 1;
	if (y >= texture->h) y = texture->h - 1;

	float wu, wv, w[2][2];
	float r, g, b;
	int c[2][2];

	wu = (float)fmod(u * texture->w, 1.0f);
	wv = (float)fmod(v * texture->h, 1.0f);

	w[0][0] = (1.0f - wu) * (1.0f - wv);
	w[0][1] = wu * (1.0f - wv);
	w[1][0] = (1.0f - wu) * wv;
	w[1][1] = wu * wv;

	c[0][0] = gettexturepixel(x, y);
	c[0][1] = gettexturepixel(x + 1, y);
	c[1][0] = gettexturepixel(x, y + 1);
	c[1][1] = gettexturepixel(x + 1, y + 1);

	r = UNPACK_R(c[0][0]) * w[0][0] + UNPACK_R(c[0][1]) * w[0][1] + UNPACK_R(c[1][0]) * w[1][0] + UNPACK_R(c[1][1]) * w[1][1];
	g = UNPACK_G(c[0][0]) * w[0][0] + UNPACK_G(c[0][1]) * w[0][1] + UNPACK_G(c[1][0]) * w[1][0] + UNPACK_G(c[1][1]) * w[1][1];
	b = UNPACK_B(c[0][0]) * w[0][0] + UNPACK_B(c[0][1]) * w[0][1] + UNPACK_B(c[1][0]) * w[1][0] + UNPACK_B(c[1][1]) * w[1][1];

	int color = PACK_RGB((int)r, (int)g, (int)b);
	return color;
#else
	int x = (int)(u * texture->w);
	int y = (int)(v * texture->h);
	int color = gettexturepixel(x, y);
	return color;
#endif

#else
	if (u < 0 || u > 1) return 0x00ff00ff;
	if (v < 0 || v > 1) return 0x00ff00ff;

	int color = 0xffffffff;
	int x = (int)(u * texture->w);
	int y = (int)(v * texture->h);

	int b = texture->image[y * texture->pitch + (x * 3 + 0)];
	int g = texture->image[y * texture->pitch + (x * 3 + 1)];
	int r = texture->image[y * texture->pitch + (x * 3 + 2)];

	//memcpy(&color, texture->image + tv * texture->pitch + tu * 3, 3);
	color = r << 24 | g << 16 | b << 8 | 0xff;
	return color;

#endif
}


TEXTURE* Raster::loadtexture(const char* fname)
{
	FILE* fp = fopen(fname, "rb");

	BITMAPINFOHEADER bm;
	int i, off;
	if (fp == NULL)
		return NULL;
	fseek(fp, 10, SEEK_SET);

	fread(&off, 4, 1, fp);
	fread(&bm, 1, sizeof(bm), fp);
	fseek(fp, off, SEEK_SET);
	if (bm.biBitCount != 24)
		return NULL;

	TEXTURE* pTexture = new TEXTURE;
	pTexture->w = bm.biWidth;
	pTexture->h = bm.biHeight;
	pTexture->pitch = bm.biWidth * 3;
	pTexture->image = new unsigned char[pTexture->h * pTexture->pitch];

	for (i = bm.biHeight - 1; i >= 0; i--)
	{
		fread(pTexture->image + i * pTexture->pitch, 1, pTexture->pitch, fp);
		fseek(fp, (4 - ((bm.biWidth * 3) & 3)) & 3, SEEK_CUR); // skip padding bytes
	}
	fclose(fp);

	return pTexture;
}

int Raster::getpoint(int x, int y)
{
	if (x > SCREEN_XSIZE || x < 0) return 0;
	if (y > SCREEN_YSIZE || y < 0) return 0;

	return screenbuffer[(int)x + (int)y * SCREEN_XSIZE];
}

void Raster::postprocess()
{
	for (int y = 0; y < SCREEN_YSIZE-1; y++)
	{
		for (int x = 0; x < SCREEN_XSIZE-1; x++)
		{
			float r, g, b;
			int c[2][2];

			c[0][0] = screenbuffer[x + y * SCREEN_XSIZE];
			c[0][1] = screenbuffer[(x+1) + y * SCREEN_XSIZE];
			c[1][0] = screenbuffer[x + (y+1) * SCREEN_XSIZE];
			c[1][1] = screenbuffer[(x+1) + (y+1) * SCREEN_XSIZE];

			r = UNPACK_R(c[0][0]) * 0.25f + UNPACK_R(c[0][1]) * 0.25f + UNPACK_R(c[1][0]) * 0.25f + UNPACK_R(c[1][1]) * 0.25f;
			g = UNPACK_G(c[0][0]) * 0.25f + UNPACK_G(c[0][1]) * 0.25f + UNPACK_G(c[1][0]) * 0.25f + UNPACK_G(c[1][1]) * 0.25f;
			b = UNPACK_B(c[0][0]) * 0.25f + UNPACK_B(c[0][1]) * 0.25f + UNPACK_B(c[1][0]) * 0.25f + UNPACK_B(c[1][1]) * 0.25f;

			int color = PACK_RGB((int)r, (int)g, (int)b);
			screenbuffer[x + y * SCREEN_XSIZE] = color;
		}
	}

}

