
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>

#include <SDL.h>
//#include <SDL_image.h>
#include <SDL_timer.h>

#include "predef.h"
#include "math.h"

#pragma comment (lib,"SDL2.lib")
#pragma comment (lib,"SDL2main.lib")



#define NUMOFSTARS	2

////////////////////////////////////////////////////////////////////////////////////

unsigned int* screenbuffer = nullptr;
float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
float fov = 45.f * _DEGREE;

MAT camera;
MAT proj;
MAT viewport;	// viwe port

Vec3 stars[NUMOFSTARS];

////////////////////////////////////////////////////////////////////////

void InitRenderer()
{
	Vec3 eye;
	eye.x = 0;	eye.y = 0;	eye.z = 0;
	Vec3 at;
	at.x = 0;	at.y = 0;	at.z = -1;
	Vec3 up;
	up.x = 0;	up.y = 1;	up.z = 0;

	// Init camera mat
	Identity(&camera);
	//MatrixLookAtLH(&camera, eye, at, up);

	// projection mat
	Identity(&proj);
	MatrixPerspectiveFovLH(&proj, fov, aspect, 0.1f, 400);

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

////////////////////////////////////////////////////////////////////////

void InitStar()
{
/*
	for (int i = 0; i < NUMOFSTARS; i++)
	{
		float x = (rand() % 2000 * 0.1f) - 100.f; // +- 0 ~ 99
		float y = (rand() % 2000 * 0.1f) - 100.f;
		float z = rand() % 1000 * 0.1f;

		stars[i].x = x;
		stars[i].y = y;
		stars[i].z = z;
	}
*/

	stars[0].x = 0;
	stars[0].y = 0;
	stars[0].z = 1;

	stars[1].x = 0;
	stars[1].y = 2;
	stars[1].z = 1;
}

void RenderStars()
{

	for (int i = 0; i < NUMOFSTARS; i++)
	{
		Vec3 pos;

		MAT cp;
		Multiply(&cp, camera, proj);

		MAT final;
		Identity(&final);
		Multiply(&final, viewport, cp);

		Transform(&pos, stars[i], final);

		float x, y, z, w, rhw;

		x = pos.x * final._11 + pos.y * final._21 + pos.z * final._31 + final._41;
		y = pos.x * final._12 + pos.y * final._22 + pos.z * final._32 + final._42;
		z = pos.x * final._13 + pos.y * final._23 + pos.z * final._33 + final._43;
		w = pos.x * final._14 + pos.y * final._24 + pos.z * final._34 + final._44;

		rhw = 1.0f / w;

		pos.x = x * rhw;
		pos.y = y * rhw;
		pos.z = z * rhw;
		//pos.rhw = rhw;

		DrawPoint((int)pos.x, (int)pos.y, 0xFFFFFFFF);
	}
}

////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(AppName,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	InitStar();

	while (1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) exit(0);
		}

// 		for (int y = 0; y < SCREEN_YSIZE; ++y)
// 			for (int x = 0; x < SCREEN_XSIZE; ++x)
// 				DrawPoint(x, y, 0xff0f0fff);

		memset((char*)screenbuffer, 0, sizeof(int) * SCREEN_XSIZE * SCREEN_YSIZE);
		
		RenderStars();

		SDL_RenderClear(renderer);
		SDL_UpdateTexture(screen_texture, NULL, screenbuffer, SCREEN_XSIZE * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete [] screenbuffer;
	
	return 0;
}