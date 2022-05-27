#ifndef RENDERER_H
#define RENDERER_H

#include "predef.h"
#include "raster.h"
#include "model.h"

struct Light
{
	Vec3 pos;
	Vec3 dir;
};


class Renderer
{
private:

	Light light;

	Raster raster;
	Model model;

	MAT view;
	MAT proj;
	MAT viewport;	// view port

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