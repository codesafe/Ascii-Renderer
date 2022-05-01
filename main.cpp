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

float* zbuffer = nullptr;

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

void ClearScreen()
{
	memset((char*)screenbuffer, 0, sizeof(int) * SCREEN_XSIZE * SCREEN_YSIZE);

	for (int i = 0; i < SCREEN_XSIZE * SCREEN_YSIZE; i++)
		zbuffer[i] = 1.0f; // -1 ~ +1
}

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
	// 0 ~ 1로 하던가 -1 ~ +1
	MatrixSetViewPort(&viewport, 0, 0, SCREEN_XSIZE, SCREEN_YSIZE, 0, 1);
	
	screenbuffer = new unsigned int[SCREEN_XSIZE * SCREEN_YSIZE * 4];
	zbuffer = new float[SCREEN_XSIZE * SCREEN_YSIZE];

	texture = LoadTexture("texture.bmp");
}

void DrawPoint(int x, int y, float z, int color)
{
	if (x > SCREEN_XSIZE || x < 0) return;
	if (y > SCREEN_YSIZE || y < 0) return;

	if (zbuffer[x + y * SCREEN_XSIZE] >= z)
	{
		screenbuffer[(int)x + (int)y * SCREEN_XSIZE] = color;
		zbuffer[x + y * SCREEN_XSIZE] = z;
	}
}

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

	maxx = maxx > SCREEN_XSIZE ? SCREEN_XSIZE : maxx;
	maxy = maxy > SCREEN_YSIZE ? SCREEN_YSIZE : maxy;

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
				float z = v1.pos.z * w0 + v2.pos.z * w1 + v3.pos.z * w2;
				int color = ReadTexel(u, v);
				DrawPoint(x, y, z, color);
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

	vertise2[0].pos.x = 0;
	vertise2[0].pos.y = 1;
	vertise2[0].pos.z = 0;
	vertise2[0].uv.u = 0;
	vertise2[0].uv.v = 1;

	vertise2[1].pos.x = -1;
	vertise2[1].pos.y = -1;
	vertise2[1].pos.z = 0;
	vertise2[1].uv.u = 0.5f;
	vertise2[1].uv.v = 0;

	vertise2[2].pos.x = 1;
	vertise2[2].pos.y = -1;
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
	MatrixRotationY(&rot, r * _DEGREE);
	//MatRotate(&rot, 1, 1, 1, r * _DEGREE);

	Vec3 tri[NUMOFVERTEX];
	for (int i = 0; i < NUMOFVERTEX; i++)
		tri[i] = rot * vertex[i].pos;

	final = view * proj *viewport;

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

		ClearScreen();

		RenderTriangle(vertise, r);
		r++;

		r1 = r+90;
		RenderTriangle(vertise2, r1);

		SDL_RenderClear(renderer);
		SDL_UpdateTexture(screen_texture, NULL, screenbuffer, SCREEN_XSIZE * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete [] screenbuffer;
	delete[] zbuffer;
	
	return 0;
}