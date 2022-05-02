#ifndef RENDERER_H
#define RENDERER_H

#include "predef.h"
#include "raster.h"
#include "model.h"

class Renderer
{
private:
	Raster raster;
	Model model;

	MAT view;
	MAT proj;
	MAT viewport;	// view port

public :
	Renderer();
	~Renderer();

	unsigned int* getscreenbuffer();

	void init();
	void clearscreen();
	void render();

};





#endif