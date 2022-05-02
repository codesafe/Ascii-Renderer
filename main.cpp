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



char gradientTbl2[] = { "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. " };
char gradientTbl[] = { 36, 64, 66, 37, 56, 38, 87, 77, 35, 42, 111, 97, 104, 107, 98, 100, 112, 113, 119,
109, 90, 79, 48, 81, 76, 67, 74, 85, 89, 88, 122, 99, 118, 117, 110, 120, 114, 106, 102, 116, 47, 92,
124, 40, 41, 49, 123, 125, 91, 93, 63, 45, 95, 43, 126, 60, 62, 105, 33, 108, 73, 59, 58, 44, 34, 94, 96, 39, 46, 32 };
//ASCIIGRADIENT = ' .:`\'-,;~_!"?c\\^<>|=sr1Jo*(C)utia3zLvey75jST{lx}IfY]qp9n0G62Vk8UXhZ4bgdPEKA$wQm&#HDR@WNBM'


Renderer renderer;


void InitRenderer()
{
	renderer.init();
}

////////////////////////////////////////////////////////////////////////

/*


void RenderTriangle(Vertex *vertex, float r)
{
	Vec3 out[NUMOFVERTEX];
	Vec4 pos;
	MAT final;
	MAT rot;
		
	Identity(&rot);
	MatrixRotationZ(&rot, r * _DEGREE);
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



	Vertex t[NUMOFVERTEX];
	t[0].pos = out[0];
	t[0].uv = vertex[0].uv;

	t[1].pos = out[1];
	t[1].uv = vertex[1].uv;

	t[2].pos = out[2];
	t[2].uv = vertex[2].uv;



	DrawTriangle(t[0], t[1], t[2]);


}*/

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

	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_PRESENTVSYNC);

	SDL_RenderSetLogicalSize(sdl_renderer, SCREEN_XSIZE, SCREEN_YSIZE);
	SDL_RenderSetIntegerScale(sdl_renderer, SDL_TRUE);

	SDL_Texture* screen_texture = SDL_CreateTexture(sdl_renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		SCREEN_XSIZE, SCREEN_YSIZE);

	InitRenderer();

	while (1)
	{


		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) exit(0);

			switch (event.type) 
			{
// 				case SDL_KEYDOWN:
// 					switch (event.key.keysym.sym) 
// 					{
// 					case SDLK_UP:
// 						fov++;
// 						Identity(&proj);
// 						MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, MIN_Z, MAX_Z);
// 						printf("FOV %f \n", fov);
// 						break;
// 
// 					case SDLK_DOWN:
// 						fov--;
// 						Identity(&proj);
// 						MatrixPerspectiveFovLH(&proj, fov * _DEGREE, aspect, MIN_Z, MAX_Z);
// 						printf("FOV %f \n", fov);
// 						break;
// 
// 					case SDLK_LEFT:
// 						eye.x -= 0.1f;
// 						at.x -= 0.1f;
// 						Identity(&view);
// 						MatrixLookAtLH(&view, eye, at, up);
// 						printf("EYE X %f \n", eye.x);
// 						break;
// 
// 					case SDLK_RIGHT:
// 						eye.x += 0.1f;
// 						at.x += 0.1f;
// 						Identity(&view);
// 						MatrixLookAtLH(&view, eye, at, up);
// 						printf("EYE X %f \n", eye.x);
// 
// 						break;
// 
// 					default:
// 						break;
// 					}
// 					break;

				case SDL_KEYUP:
					break;

				default:
					break;
			}
		}

		renderer.clearscreen();
		renderer.render();

		SDL_RenderClear(sdl_renderer);
		SDL_UpdateTexture(screen_texture, NULL, renderer.getscreenbuffer(), SCREEN_XSIZE * 4);
		SDL_RenderCopy(sdl_renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(sdl_renderer);
	}

	return 0;
}