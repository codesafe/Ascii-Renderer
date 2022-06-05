#ifndef RENDERER_H
#define RENDERER_H

#include "predef.h"
#include "raster.h"
#include "model.h"

class Renderer
{
private:

	Light light;

	Raster raster;
	Model model;

	MAT view;
	MAT proj;
	MAT viewport;	// view port


	Shader shader;

	bool togglev;
public :

	Renderer();
	~Renderer();

	unsigned int* getscreenbuffer();

	void init();
	void clearscreen();
	void render();

	void toggle();

};





#endif