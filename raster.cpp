#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>

#include "raster.h"

Raster::Raster()
{
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
		screenbuffer[(int)x + (int)y * SCREEN_XSIZE] = color;
		zbuffer[x + y * SCREEN_XSIZE] = z;
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


float Raster::edge(const Vec3& a, const Vec3& b, const Vec3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void Raster::drawtriangle(Vertex& v1, Vertex& v2, Vertex& v3)
{
	float y1 = v1.pos.y;
	float y2 = v2.pos.y;
	float y3 = v3.pos.y;

	float x1 = v1.pos.x;
	float x2 = v2.pos.x;
	float x3 = v3.pos.x;

	// seem ����
	int minx = (int)ceil(min(min(x1, x2) - 0.5f, x3) - 0.5f);
	int maxx = (int)ceil(max(max(x1, x2) + 0.5f, x3) + 0.5f);
	int miny = (int)ceil(min(min(y1, y2) - 0.5f, y3) - 0.5f);
	int maxy = (int)ceil(max(max(y1, y2) + 0.5f, y3) + 0.5f);

	float minu = min(min(v1.uv.u, v2.uv.u), v3.uv.u);
	float maxu = max(max(v1.uv.u, v2.uv.u), v3.uv.u);
	float minv = min(min(v1.uv.v, v2.uv.v), v3.uv.v);
	float maxv = max(max(v1.uv.v, v2.uv.v), v3.uv.v);

	float area = edge(v1.pos, v2.pos, v3.pos);

	maxx = maxx > SCREEN_XSIZE ? SCREEN_XSIZE : maxx;
	maxy = maxy > SCREEN_YSIZE ? SCREEN_YSIZE : maxy;

	for (int y = miny; y < maxy; y++)
	{
		for (int x = minx; x < maxx; x++)
		{
			Vec3 p((float)x, (float)y, 0);
			float w0 = edge(v2.pos, v3.pos, p);
			float w1 = edge(v3.pos, v1.pos, p);
			float w2 = edge(v1.pos, v2.pos, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;

				float u = v1.uv.u * w0 + v2.uv.u * w1 + v3.uv.u * w2;
				float v = v1.uv.v * w0 + v2.uv.v * w1 + v3.uv.v * w2;
				float z = v1.pos.z * w0 + v2.pos.z * w1 + v3.pos.z * w2;
				int color = readtexel(u, v);
				drawpoint(x, y, z, color);
			}
		}
	}
}

int Raster::readtexel(float u, float v)
{
	int color = 0xffffffff;
	int x = (int)(u * texture->w);
	int y = (int)(v * texture->h);

	int b = texture->image[y * texture->pitch + (x * 3 + 0)];
	int g = texture->image[y * texture->pitch + (x * 3 + 1)];
	int r = texture->image[y * texture->pitch + (x * 3 + 2)];

	//memcpy(&color, texture->image + tv * texture->pitch + tu * 3, 3);
	color = r << 24 | g << 16 | b << 8 | 0xff;
	return color;
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


