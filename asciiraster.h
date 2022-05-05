#ifndef ASCII_RASTER_H
#define ASCII_RASTER_H

#include "predef.h"

class AsciiRaster
{
private:
	HANDLE hConsole;
	CHAR_INFO* BackBuffer;
	unsigned char *buffer;

	int WIDTH_SCREEN;
	int HEIGHT_SCREEN;

public:
	AsciiRaster();
	~AsciiRaster();

	void init();
	void clearscreen();
	void drawpoint(int x, int y, char symb, int color);
	void resize(unsigned int* source, int sourceWidth, int sourceHeight);
	void render(unsigned int* renderbuffer);
};


#endif