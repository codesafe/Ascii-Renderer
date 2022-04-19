
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



#define NUMOFSTARS	3
#define MAX_Z		100

////////////////////////////////////////////////////////////////////////////////////

unsigned int* screenbuffer = nullptr;
float aspect = (float)SCREEN_XSIZE / (float)SCREEN_YSIZE;
float fov = 45.f ;

MAT view;
MAT proj;
MAT viewport;	// viwe port

Vec3 stars[NUMOFSTARS];

////////////////////////////////////////////////////////////////////////

void InitRenderer()
{
	Vec3 eye;
	eye.x = 0;	eye.y = 0;	eye.z = 0;
	Vec3 at;
	at.x = 0;	at.y = 0;	at.z = 1;
	Vec3 up;
	up.x = 0;	up.y = 1;	up.z = 0;

	// Init view mat
	Identity(&view);
	MatrixLookAtLH(&view, eye, at, up);

	// projection mat
	Identity(&proj);
	MatrixPerspectiveFovLH(&proj, fov*_DEGREE, aspect, 0.1f, MAX_Z);

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

Vec3 GetNewPos()
{
	float range = 20.f;
	Vec3 pos;
	pos.x = (rand() % (int)(range * 20) * 0.1f) - range;
	pos.y = (rand() % (int)(range * 20) * 0.1f) - range;
	pos.z = (float)(rand() % MAX_Z);

	return pos;
}

void InitStar()
{
// 	for (int i = 0; i < NUMOFSTARS; i++)
// 		stars[i] = GetNewPos();

	stars[0].x = 0;
	stars[0].y = 0;
	stars[0].z = 1;

	stars[1].x = 10;
	stars[1].y = 0;
	stars[1].z = 20;

	stars[2].x = 0;
	stars[2].y = -1;
	stars[2].z = MAX_Z;
}

void RenderStars()
{

	for (int i = 0; i < NUMOFSTARS; i++)
	{
		Vec4 pos;
		Vec4 s(stars[i].x, stars[i].y, stars[i].z, 1);

		MAT vp;
		MAT final;
		Multiply(&vp, view, proj);
		Multiply(&final, vp, viewport);
		Transform4(&pos, s, final);

		Vec3 out;
		PerspectiveDivide(&out, pos);

		//unsigned char bright = 255 - (unsigned char)( (stars[i].z / (float)MAX_Z) * 255 );
		unsigned char bright = (unsigned char)(out.z * 255.f);
		int color = bright << 24 | bright << 16 | bright << 8 | 0xff;

		DrawPoint((int)out.x, (int)out.y, color/*0x0000FFFF*/);

//		stars[i].z = stars[i].z - 0.1f;

		if(stars[i].z < 0)
			stars[i] = GetNewPos();
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
						MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, 0.1f, 400);
						printf("FOV %f \n", fov);
						break;
					case SDLK_DOWN:
						fov--;
						Identity(&proj);
						MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, 0.1f, 400);
						printf("FOV %f \n", fov);

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