#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <ctime>

#include <SDL.h>
//#include <SDL_image.h>
#include <SDL_timer.h>
#include <functional>

#include "predef.h"
#include "math.h"
#include "renderer.h"
#include "asciiraster.h"

#pragma comment (lib,"SDL2.lib")
#pragma comment (lib,"SDL2main.lib")



////////////////////////////////////////////////////////////////////////

#ifdef ASCII_RENDER_ONLY

int main(int argc, char* argv[])
{
	Renderer renderer;
	renderer.init();

	AsciiRaster ascii;
	ascii.init();

	while (1) 
	{
		renderer.clearscreen();
		renderer.render();

		ascii.clearscreen();
		ascii.render(renderer.getscreenbuffer());
	}

	return 0;
}

#else

int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(AppName,
		300,300,
		//SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_XSIZE, SCREEN_YSIZE, 0);
		//SDL_WINDOW_RESIZABLE);

	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	SDL_RenderSetLogicalSize(sdl_renderer, SCREEN_XSIZE, SCREEN_YSIZE);
	SDL_RenderSetIntegerScale(sdl_renderer, SDL_TRUE);

	SDL_Texture* screen_texture = SDL_CreateTexture(sdl_renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		SCREEN_XSIZE, SCREEN_YSIZE);

	////////////////////////////////////////////////////////////////////////

	Renderer renderer;
	renderer.init();

#ifdef ASCII_RENDER
	AsciiRaster ascii;
	ascii.init();
#endif

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

						break;

					case SDLK_DOWN:
						break;

					case SDLK_LEFT:
						break;

					case SDLK_RIGHT:
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

		renderer.clearscreen();
		renderer.render();

		SDL_RenderClear(sdl_renderer);
		SDL_UpdateTexture(screen_texture, NULL, renderer.getscreenbuffer(), SCREEN_XSIZE * 4);
		SDL_RenderCopy(sdl_renderer, screen_texture, NULL, NULL);
		SDL_RenderPresent(sdl_renderer);

#ifdef ASCII_RENDER
		ascii.clearscreen();
		ascii.render(renderer.getscreenbuffer());
#endif

	}

	return 0;
}

#endif