
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

////////////////////////////////////////////////////////////////////////


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
	// using pointers so we can swap (for sorting purposes)
	const Vec3* pv0 = &v0;
	const Vec3* pv1 = &v1;
	const Vec3* pv2 = &v2;

	// sorting vertices by y
	if (pv1->y < pv0->y) std::swap(pv0, pv1);
	if (pv2->y < pv1->y) std::swap(pv1, pv2);
	if (pv1->y < pv0->y) std::swap(pv0, pv1);

	if (pv0->y == pv1->y)
	{
		// natural flat top
		// sorting top vertices by x
		if (pv1->x < pv0->x) std::swap(pv0, pv1);
		DrawFlatTop(*pv0, *pv1, *pv2);
	}
	else if (pv1->y == pv2->y)
	{
		// natural flat bottom
		// sorting bottom vertices by x
		if (pv2->x < pv1->x) std::swap(pv1, pv2);
		DrawFlatBottom(*pv0, *pv1, *pv2);
	}
	else
	{
		// find splitting vertex
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

////////////////////////////////////////////////////////////////////////


void InitTriangle()
{
	vertise[0].pos.x = -1;
	vertise[0].pos.y = 1;
	vertise[0].pos.z = 0;

	vertise[1].pos.x = 1;
	vertise[1].pos.y = 1;
	vertise[1].pos.z = 0;

	vertise[2].pos.x = 0;
	vertise[2].pos.y = -1;
	vertise[2].pos.z = 0;
}

float r = 0;
void RenderTriangle()
{
	//for (int i = 0; i < NUMOFVERTEX; i++)
	{
		Vec3 out[NUMOFVERTEX];
		Vec4 pos;
		MAT final;
		MAT rot;
		
		Identity(&rot);
		MatrixRotationY(&rot, (r+=1) * _DEGREE);
		//MatRotate(&rot, 1, 1, 1, (r++) * _DEGREE);

		Vec3 tri[NUMOFVERTEX];
		for (int i = 0; i < NUMOFVERTEX; i++)
			tri[i] = rot * vertise[i].pos;
			//Transform(&tri[i], vertise[i].pos, rot);

		//Multiply(&final, view, proj);
		//Multiply(&final, final, viewport);
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

		DrawTriangle(out[0], out[1], out[2]);

		//float z = pos.z / pos.w;
		int color = 0x00FF00FF;
		DrawPoint((int)out[0].x, (int)out[0].y, color);
		DrawPoint((int)out[1].x, (int)out[1].y, color);
		DrawPoint((int)out[2].x, (int)out[2].y, color);

	}
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
		
		RenderTriangle();

		SDL_RenderClear(renderer);
		SDL_UpdateTexture(screen_texture, NULL, screenbuffer, SCREEN_XSIZE * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete [] screenbuffer;
	
	return 0;
}