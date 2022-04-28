#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>

#include <SDL.h>
//#include <SDL_image.h>
#include <SDL_timer.h>
#include <functional>

#include "predef.h"
#include "math.h"
#include "renderer.h"



#pragma comment (lib,"SDL2.lib")
#pragma comment (lib,"SDL2main.lib")



#define MIN_Z		0.1f
#define MAX_Z		10

#define NUMOFVERTEX	3

////////////////////////////////////////////////////////////////////////////////////

unsigned int* screenbuffer = nullptr;
float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
float fov = 45.f ;

MAT view;
MAT proj;
MAT viewport;	// viwe port

// Camra
Vec3 eye;
Vec3 at;
Vec3 up;

Vertex vertise[NUMOFVERTEX];
Vertex vertise2[NUMOFVERTEX];
TEXTURE *texture;

char gradientTbl2[] = { "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. " };
char gradientTbl[] = { 36, 64, 66, 37, 56, 38, 87, 77, 35, 42, 111, 97, 104, 107, 98, 100, 112, 113, 119,
109, 90, 79, 48, 81, 76, 67, 74, 85, 89, 88, 122, 99, 118, 117, 110, 120, 114, 106, 102, 116, 47, 92,
124, 40, 41, 49, 123, 125, 91, 93, 63, 45, 95, 43, 126, 60, 62, 105, 33, 108, 73, 59, 58, 44, 34, 94, 96, 39, 46, 32 };


//ASCIIGRADIENT = ' .:`\'-,;~_!"?c\\^<>|=sr1Jo*(C)utia3zLvey75jST{lx}IfY]qp9n0G62Vk8UXhZ4bgdPEKA$wQm&#HDR@WNBM'

////////////////////////////////////////////////////////////////////////


TEXTURE* LoadTexture(const char* fname)
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

int ReadTexel(float u, float v)
{
	int color = 0xffffffff;
	int x = (int)(u * texture->w);
	int y = (int)(v * texture->h);

	int b = texture->image[y * texture->pitch + (x * 3 + 0)];
	int g = texture->image[y * texture->pitch + (x * 3 + 1)];
	int r = texture->image[y * texture->pitch + (x * 3 + 2)];

	if (b > 0)
		b = b;

	//memcpy(&color, texture->image + tv * texture->pitch + tu * 3, 3);
	color = r << 24 | g << 16 | b << 8 | 0xff;
	return color;
}


void InitRenderer()
{
	eye.x = 0;	eye.y = 0;	eye.z = -5;
	at.x = 0;	at.y = 0;	at.z = 1;
	up.x = 0;	up.y = 1;	up.z = 0;

	// Init view mat
	Identity(&view);
	MatrixLookAtLH(&view, eye, at, up);

	// projection mat
	Identity(&proj);
	MatrixPerspectiveFovLH(&proj, fov*_DEGREE, aspect, MIN_Z, MAX_Z);

	Identity(&viewport);
	MatrixSetViewPort(&viewport, 0, 0, SCREEN_XSIZE, SCREEN_YSIZE);
	
	screenbuffer = new unsigned int[SCREEN_XSIZE * SCREEN_YSIZE * 4];

	texture = LoadTexture("texture.bmp");
}



void DrawPoint(int x, int y, int color)
{
	if (x > SCREEN_XSIZE || x < 0) return;
	if (y > SCREEN_YSIZE || y < 0) return;

	screenbuffer[x + y * SCREEN_XSIZE] = color;
}

void DrawLine(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;

	while (DrawPoint(x0, y0, 0xFFFFFFFF), x0 != x1 || y0 != y1) 
	{
		int e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}
/*


void DrawFlatTop(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	float m0 = (v2.x - v0.x) / (v2.y - v0.y);
	float m1 = (v2.x - v1.x) / (v2.y - v1.y);

	int ys = (int)ceil(v0.y - 0.5f);	// y start
	int ye = (int)ceil(v2.y - 0.5f);	// y end

	for (int y = ys; y < ye; y++)
	{
		float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

// 		int xs = (int)ceil(px0 - 0.5f);	// x start
// 		int xe = (int)ceil(px1 - 0.5f);	// x end

		int xs = (int)(px0 - 0.5f);	// x start
		int xe = (int)(px1 - 0.5f);	// x end

		for (int x = xs; x < xe; x++)
		{
			DrawPoint(x, y, 0xFFFFFFFF);
		}
	}
}


void DrawFlatBottom(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	float m0 = (v1.x - v0.x) / (v1.y - v0.y);
	float m1 = (v2.x - v0.x) / (v2.y - v0.y);

	const int ys = (int)ceil(v0.y - 0.5f);
	const int ye = (int)ceil(v2.y - 0.5f);

	for (int y = ys; y < ye; y++)
	{
		float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

		//int xs = (int)ceil(px0 - 0.5f);
		//int xe = (int)ceil(px1 - 0.5f);
		int xs = (int)(px0 - 0.5f);
		int xe = (int)(px1 - 0.5f);

		for (int x = xs; x < xe; x++)
		{
			DrawPoint(x, y, 0x00FFFFFF);
		}
	}
}


void DrawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	const Vec3* pv0 = &v0;
	const Vec3* pv1 = &v1;
	const Vec3* pv2 = &v2;

	if (pv1->y < pv0->y) std::swap(pv0, pv1);
	if (pv2->y < pv1->y) std::swap(pv1, pv2);
	if (pv1->y < pv0->y) std::swap(pv0, pv1);

	if (pv0->y == pv1->y)
	{
		if (pv1->x < pv0->x) std::swap(pv0, pv1);
		DrawFlatTop(*pv0, *pv1, *pv2);
	}
	else if (pv1->y == pv2->y)
	{
		if (pv2->x < pv1->x) std::swap(pv1, pv2);
		DrawFlatBottom(*pv0, *pv1, *pv2);
	}
	else
	{
		const float alphaSplit = (pv1->y - pv0->y) / (pv2->y - pv0->y);
		const Vec3 vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

		if (pv1->x < vi.x) // major right
		{
			DrawFlatBottom(*pv0, *pv1, vi);
			DrawFlatTop(*pv1, vi, *pv2);
		}
		else // major left
		{
			DrawFlatBottom(*pv0, vi, *pv1);
			DrawFlatTop(vi, *pv1, *pv2);
		}
	}
}
*/


// Half Space algorithms
/*
void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	float y1 = v1.pos.y;   
	float y2 = v2.pos.y;
	float y3 = v3.pos.y;

	float x1 = v1.pos.x;
	float x2 = v2.pos.x;
	float x3 = v3.pos.x;

	int minx = (int)min(min(x1, x2), x3);
	int maxx = (int)max(max(x1, x2), x3);
	int miny = (int)min(min(y1, y2), y3);
	int maxy = (int)max(max(y1, y2), y3);

	float minu = min(min(v1.uv.u, v2.uv.u), v3.uv.u);
	float maxu = max(max(v1.uv.u, v2.uv.u), v3.uv.u);
	float minv = min(min(v1.uv.v, v2.uv.v), v3.uv.v);
	float maxv = max(max(v1.uv.v, v2.uv.v), v3.uv.v);


	for (int y = miny; y < maxy; y++)
	{
		for (int x = minx; x < maxx; x++)
		{
			if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) > 0 &&
				(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) > 0 &&
				(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) > 0)
			{
				float t0 = (x - minx + 0.5f) / (maxx - minx);
				float t1 = (y - miny + 0.5f) / (maxy - miny);

 				float u = lerp(minu, maxu, t0);
 				float v = lerp(minv, maxv, t1);

// 				float u = ((x - minx) * (maxu - minu) / (maxx - minx)) + minu;
// 				float v = ((y - miny) * (maxv - minv) / (maxy - miny)) + minv;

				//int color = 0xffffffff;
				int color = ReadTexel(u, v);

				DrawPoint(x, y, color);
			}
		}
	}
}
*/

float edgeFunction(const Vec3& a, const Vec3& b, const Vec3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	float y1 = v1.pos.y;
	float y2 = v2.pos.y;
	float y3 = v3.pos.y;

	float x1 = v1.pos.x;
	float x2 = v2.pos.x;
	float x3 = v3.pos.x;

	int minx = (int)min(min(x1, x2), x3);
	int maxx = (int)max(max(x1, x2), x3);
	int miny = (int)min(min(y1, y2), y3);
	int maxy = (int)max(max(y1, y2), y3);

	float minu = min(min(v1.uv.u, v2.uv.u), v3.uv.u);
	float maxu = max(max(v1.uv.u, v2.uv.u), v3.uv.u);
	float minv = min(min(v1.uv.v, v2.uv.v), v3.uv.v);
	float maxv = max(max(v1.uv.v, v2.uv.v), v3.uv.v);

	float area = edgeFunction(v1.pos, v2.pos, v3.pos);

	for (int y = miny; y < maxy; y++)
	{
		for (int x = minx; x < maxx; x++)
		{
			if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) > 0 &&
				(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) > 0 &&
				(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) > 0)
			{
				Vec3 p((float)x, (float)y, 0);
				float w0 = edgeFunction(v2.pos, v3.pos, p);
				float w1 = edgeFunction(v3.pos, v1.pos, p);
				float w2 = edgeFunction(v1.pos, v2.pos, p);

				w0 /= area;
				w1 /= area;
				w2 /= area;

				float u = v1.uv.u * w0 + v2.uv.u * w1 + v3.uv.u * w2;
				float v = v1.uv.v * w0 + v2.uv.v * w1 + v3.uv.v * w2;
				int color = ReadTexel(u, v);
				DrawPoint(x, y, color);
			}
		}
	}
}




////////////////////////////////////////////////////////////////////////


void InitTriangle()
{
	vertise[0].pos.x = -1;
	vertise[0].pos.y = 1;
	vertise[0].pos.z = 0;
	vertise[0].uv.u = 0;
	vertise[0].uv.v = 1;

	vertise[1].pos.x = 0;
	vertise[1].pos.y = -1;
	vertise[1].pos.z = 0;
	vertise[1].uv.u = 0.5f;
	vertise[1].uv.v = 0;

	vertise[2].pos.x = 1;
	vertise[2].pos.y = 1;
	vertise[2].pos.z = 0;
	vertise[2].uv.u = 1;
	vertise[2].uv.v = 1;

	/// 

	vertise2[0].pos.x = -1;
	vertise2[0].pos.y = 1;
	vertise2[0].pos.z = 0;
	vertise2[0].uv.u = 0;
	vertise2[0].uv.v = 1;

	vertise2[1].pos.x = 0;
	vertise2[1].pos.y = -1;
	vertise2[1].pos.z = 0;
	vertise2[1].uv.u = 0.5f;
	vertise2[1].uv.v = 0;

	vertise2[2].pos.x = 1;
	vertise2[2].pos.y = 1;
	vertise2[2].pos.z = 0;
	vertise2[2].uv.u = 1;
	vertise2[2].uv.v = 1;
}


void RenderTriangle(Vertex *vertex, float r)
{
	Vec3 out[NUMOFVERTEX];
	Vec4 pos;
	MAT final;
	MAT rot;
		
	Identity(&rot);
	//MatrixRotationY(&rot, r * _DEGREE);
	MatRotate(&rot, 1, 1, 1, r * _DEGREE);

	Vec3 tri[NUMOFVERTEX];
	for (int i = 0; i < NUMOFVERTEX; i++)
		tri[i] = rot * vertex[i].pos;

	final = view * proj * viewport;

	for (int i = 0; i < NUMOFVERTEX; i++)
	{
		Vec3 t(tri[i].x, tri[i].y, tri[i].z);
		Transform4(&pos, t, final);
		PerspectiveDivide(&out[i], pos);
	}


// 		DrawLine(out[0].x, out[0].y, out[1].x, out[1].y);
// 		DrawLine(out[1].x, out[1].y, out[2].x, out[2].y);
// 		DrawLine(out[2].x, out[2].y, out[0].x, out[0].y);

	Vertex t[NUMOFVERTEX];
	t[0].pos = out[0];
	t[0].uv = vertex[0].uv;

	t[1].pos = out[1];
	t[1].uv = vertex[1].uv;

	t[2].pos = out[2];
	t[2].uv = vertex[2].uv;

	//DrawTriangle(out[0], out[1], out[2]);

	DrawTriangle(t[0], t[1], t[2]);

// 	int color = 0x00FF00FF;
// 	DrawPoint((int)out[0].x, (int)out[0].y, color);
// 	DrawPoint((int)out[1].x, (int)out[1].y, color);
// 	DrawPoint((int)out[2].x, (int)out[2].y, color);
}

////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(AppName,
		300,300,
		//SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_XSIZE, SCREEN_YSIZE, 0);
		//SDL_WINDOW_RESIZABLE);

	SDL_Renderer* renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_PRESENTVSYNC);

	SDL_RenderSetLogicalSize(renderer, SCREEN_XSIZE, SCREEN_YSIZE);
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

	SDL_Texture* screen_texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		SCREEN_XSIZE, SCREEN_YSIZE);

	InitRenderer();
	InitTriangle();

	float r = 0;
	float r1 = 180;

	while (1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) exit(0);

			switch (event.type) 
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) 
					{
					case SDLK_UP:
						fov++;
						Identity(&proj);
						MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, MIN_Z, MAX_Z);
						printf("FOV %f \n", fov);
						break;

					case SDLK_DOWN:
						fov--;
						Identity(&proj);
						MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, MIN_Z, MAX_Z);
						printf("FOV %f \n", fov);
						break;

					case SDLK_LEFT:
						eye.x -= 0.1f;
						Identity(&view);
						MatrixLookAtLH(&view, eye, at, up);
						printf("EYE X %f \n", eye.x);
						break;

					case SDLK_RIGHT:
						eye.x += 0.1f;
						Identity(&view);
						MatrixLookAtLH(&view, eye, at, up);
						printf("EYE X %f \n", eye.x);

						break;

					default:
						break;
					}
					break;

				case SDL_KEYUP:
					break;

				default:
					break;
			}
		}

// 		for (int y = 0; y < SCREEN_YSIZE; ++y)
// 			for (int x = 0; x < SCREEN_XSIZE; ++x)
// 				DrawPoint(x, y, 0xff0f0fff);

		memset((char*)screenbuffer, 0, sizeof(int) * SCREEN_XSIZE * SCREEN_YSIZE);
		
		RenderTriangle(vertise, r);
		r++;
		RenderTriangle(vertise2, r1);
		r1+=2;

		SDL_RenderClear(renderer);
		SDL_UpdateTexture(screen_texture, NULL, screenbuffer, SCREEN_XSIZE * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete [] screenbuffer;
	
	return 0;
}