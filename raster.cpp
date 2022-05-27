#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>

#include "raster.h"

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
	texture = loadtexture("texture.bmp");
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

float Raster::edge(const Vec4& a, const Vec4& b, const Vec4& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

Vec3 Raster::getfacenormal(Vec3& v0, Vec3& v1, Vec3& v2)
{
	// Calculate triangle normal for backface culling
	Vec3 s1 = v0 - v2;
	Vec3 s2 = v0 - v1;

	Vec3 facenormal;
	Cross(&facenormal, s1, s2);
	facenormal.norm();
	return facenormal;
}

void Raster::drawtriangle(Vertex& v1, Vertex& v2, Vertex& v3, float bright)
{
	Vec3 p1 = Vec3(v1.pos);
	Vec3 p2 = Vec3(v2.pos);
	Vec3 p3 = Vec3(v3.pos);

	// back face cull
	Vec3 facenormal = getfacenormal(p1, p2, p3);
	Vec3 t = Vec3(0, 0, 1.0f);
	if (facenormal.dot(t) < 0)
		return;

#ifdef WIRE_FRAME
	drawTriangleOutline(v1, v2, v3, 0xFFFFFFFF);
#else

	float y1 = v1.pos.y;
	float y2 = v2.pos.y;
	float y3 = v3.pos.y;

	float x1 = v1.pos.x;
	float x2 = v2.pos.x;
	float x3 = v3.pos.x;

	// seem Á¦°Å
	int minx = (int)ceil(min(min(x1, x2) - 0.5f, x3) - 0.5f);
	int maxx = (int)ceil(max(max(x1, x2) + 0.5f, x3) + 0.5f);
	int miny = (int)ceil(min(min(y1, y2) - 0.5f, y3) - 0.5f);
	int maxy = (int)ceil(max(max(y1, y2) + 0.5f, y3) + 0.5f);

	float minu = min(min(v1.uv.u, v2.uv.u), v3.uv.u);
	float maxu = max(max(v1.uv.u, v2.uv.u), v3.uv.u);
	float minv = min(min(v1.uv.v, v2.uv.v), v3.uv.v);
	float maxv = max(max(v1.uv.v, v2.uv.v), v3.uv.v);

	float area = edge(v1.pos, v2.pos, v3.pos);

	minx = minx < 0 ? 0 : minx;
	miny = miny < 0 ? 0 : miny;
	maxx = maxx > SCREEN_XSIZE ? SCREEN_XSIZE : maxx;
	maxy = maxy > SCREEN_YSIZE ? SCREEN_YSIZE : maxy;

#ifdef PERSPETIVE_CORRECT

	v1.uv.u = v1.uv.u / v1.pos.w;
	v1.uv.v = v1.uv.v / v1.pos.w;

	v2.uv.u = v2.uv.u / v2.pos.w;
	v2.uv.v = v2.uv.v / v2.pos.w;

	v3.uv.u = v3.uv.u / v3.pos.w;
	v3.uv.v = v3.uv.v / v3.pos.w;

	v1.pos.w = 1.0f / v1.pos.w;
	v2.pos.w = 1.0f / v2.pos.w;
	v3.pos.w = 1.0f / v3.pos.w;

#endif

#ifdef LIGHTING
	Vec3 light = Vec3(-10, -13, 3.0f);
	light.norm();

	v1.normal.norm();
	v2.normal.norm();
	v3.normal.norm();

#endif

	for (int y = miny; y < maxy; y++)
	{
		for (int x = minx; x < maxx; x++)
		{
			Vec4 p((float)x, (float)y, 0, 1);
			float w0 = edge(v2.pos, v3.pos, p);
			float w1 = edge(v3.pos, v1.pos, p);
			float w2 = edge(v1.pos, v2.pos, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;

#ifdef PERSPETIVE_CORRECT
				float w = 1.0f / (v1.pos.w * w0 + v2.pos.w * w1 + v3.pos.w * w2);

				float u = v1.uv.u * w0 + v2.uv.u * w1 + v3.uv.u * w2;
				float v = v1.uv.v * w0 + v2.uv.v * w1 + v3.uv.v * w2;
				float z = v1.pos.z * w0 + v2.pos.z * w1 + v3.pos.z * w2;

				u *= w;
				v *= w;

#else
				float u = v1.uv.u * w0 + v2.uv.u * w1 + v3.uv.u * w2;
				float v = v1.uv.v * w0 + v2.uv.v * w1 + v3.uv.v * w2;
				float z = v1.pos.z * w0 + v2.pos.z * w1 + v3.pos.z * w2;
#endif

				int color = readtexel(u, v);

#ifdef LIGHTING
				float nx = (v1.normal.x * w0 + v2.normal.x * w1 + v3.normal.x * w2) * w;
				float ny = (v1.normal.y * w0 + v2.normal.y * w1 + v3.normal.y * w2) * w;
				float nz = (v1.normal.z * w0 + v2.normal.z * w1 + v3.normal.z * w2) * w;

				Vec3 nn(nx, ny, nz);
				float shading = -nn.dot(light);

 				shading = shading < 0 ? 0 : shading;
 				shading = shading > 1 ? 1 : shading;

				if (togglev == false)
				{
					shading = shading * 0.5f + 0.5f;
				}

				bright = shading;
#endif
				int r = (int)((float)(UNPACK_R(color)) * bright);
				int g = (int)((float)(UNPACK_G(color)) * bright);
				int b = (int)((float)(UNPACK_B(color)) * bright);

				color = (r << 24 | g << 16 | b << 8 | 0xff);
				drawpoint(x, y, z, color);
			}
		}
	}
#endif
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

