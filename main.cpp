
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

//#define TEST	1

#ifdef TEST
#define NUMOFSTARS	3
#else
#define NUMOFSTARS	100
#endif

#define MIN_Z		0.1f
#define MAX_Z		10

////////////////////////////////////////////////////////////////////////////////////

unsigned int* screenbuffer = nullptr;
float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
float fov = 45.f ;

MAT view;
MAT proj;
MAT viewport;	// viwe port

Vec3 stars[NUMOFSTARS];

// Camra
Vec3 eye;
Vec3 at;
Vec3 up;

////////////////////////////////////////////////////////////////////////



void InitRenderer()
{
	eye.x = 0;	eye.y = 0;	eye.z = 0;
	at.x = 0;	at.y = 0;	at.z = 1;
	up.x = 0;	up.y = 1;	up.z = 0;

	// Init view mat
	Identity(&view);
	MatrixLookAtLH(&view, eye, at, up);

	// projection mat
	Identity(&proj);
	MatrixPerspectiveFovLH(&proj, fov*_DEGREE, aspect, MIN_Z, MAX_Z);

	Identity(&viewport);
	MatrixSetViewPort(&viewport, 0, 0, SCREEN_XSIZE, SCREEN_YSIZE, MIN_Z, MAX_Z);
	
	screenbuffer = new unsigned int[SCREEN_XSIZE * SCREEN_YSIZE * 4];
}

void DrawPoint(int x, int y, int color)
{
	if (x > SCREEN_XSIZE || x < 0) return;
	if (y > SCREEN_YSIZE || y < 0) return;

	screenbuffer[x + y * SCREEN_XSIZE] = color;
}

////////////////////////////////////////////////////////////////////////

Vec3 GetNewPos()
{
	float range = 2.f;
	Vec3 pos;
	pos.x = (rand() % (int)(range * 2 * 10) * 0.1f) - range;
	pos.y = (rand() % (int)(range * 2 * 10) * 0.1f) - range;
	pos.z = (float)(rand() % MAX_Z);

	return pos;
}

void InitStar()
{
#ifdef TEST
	stars[0].x = 0;
	stars[0].y = 0;
	stars[0].z = 0.2f;

	stars[1].x = -1;
	stars[1].y = 0;
	stars[1].z = 4;

	stars[2].x = 0;
	stars[2].y = 10;
	stars[2].z = 9.9f;
#else
	for (int i = 0; i < NUMOFSTARS; i++)
		stars[i] = GetNewPos();
#endif
}

void RenderStars()
{
	static float camrot = 0;
	for (int i = 0; i < NUMOFSTARS; i++)
	{
		Vec4 pos;

		MAT rot;
		MatrixRotationY(&rot, camrot * _DEGREE);
		camrot += 0.005f;

		MAT _view = view * rot;
		MAT final;
		Multiply(&final, _view, proj);
		
		Multiply(&final, final, viewport);
		Transform4(&pos, stars[i], final);

		Vec4 out;
		PerspectiveDivide(&out, pos);

		//float z = pos.z / pos.w;

#ifndef TEST
		unsigned char bright = 255 - (unsigned char)( (stars[i].z / (float)MAX_Z) * 255 );
		//unsigned char bright = (unsigned char)((1-z) * 255.f);
		int color = bright << 24 | bright << 16 | bright << 8 | 0xff;
#else
		int color = 0xFFFFFFFF;
#endif
		DrawPoint((int)out.x, (int)out.y, color);

#ifndef TEST
		stars[i].z = stars[i].z - 0.04f;
		if(stars[i].z < 0)
			stars[i] = GetNewPos();
#endif
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
	InitStar();

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
						eye.x -= 0.01f;
						Identity(&view);
						MatrixLookAtLH(&view, eye, at, up);
						printf("EYE X %f \n", eye.x);
						break;

					case SDLK_RIGHT:
						eye.x += 0.01f;
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
		
		RenderStars();

		SDL_RenderClear(renderer);
		SDL_UpdateTexture(screen_texture, NULL, screenbuffer, SCREEN_XSIZE * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete [] screenbuffer;
	
	return 0;
}