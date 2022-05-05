#include <windows.h>
#include <iostream>

#include "asciiraster.h"

#define ASCII_TBL_N		71

char gradientTbl2[] = { "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. " };
char gradientTbl[] = { 36, 64, 66, 37, 56, 38, 87, 77, 35, 42, 111, 97, 104, 107, 98, 100, 112, 113, 119,
109, 90, 79, 48, 81, 76, 67, 74, 85, 89, 88, 122, 99, 118, 117, 110, 120, 114, 106, 102, 116, 47, 92,
124, 40, 41, 49, 123, 125, 91, 93, 63, 45, 95, 43, 126, 60, 62, 105, 33, 108, 73, 59, 58, 44, 34, 94, 96, 39, 46, 32 };
//ASCIIGRADIENT = ' .:`\'-,;~_!"?c\\^<>|=sr1Jo*(C)utia3zLvey75jST{lx}IfY]qp9n0G62Vk8UXhZ4bgdPEKA$wQm&#HDR@WNBM'


AsciiRaster::AsciiRaster()
{
}

AsciiRaster::~AsciiRaster()
{
}


void AsciiRaster::init()
{
	_COORD coord;
	_SMALL_RECT Rect;

	coord.X = 2;
	coord.Y = 2;

	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = 2 - 1;
	Rect.Right = 2 - 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hConsole, coord);
	SetConsoleWindowInfo(hConsole, TRUE, &Rect);

	coord.X = ASCII_XSIZE;
	coord.Y = ASCII_YSIZE;

	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = ASCII_YSIZE - 1;
	Rect.Right = ASCII_XSIZE - 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hConsole, coord);
	SetConsoleWindowInfo(hConsole, TRUE, &Rect);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	//int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	WIDTH_SCREEN = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	HEIGHT_SCREEN = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	BackBuffer = new CHAR_INFO[WIDTH_SCREEN * HEIGHT_SCREEN];

	clearscreen();

	hConsole = CreateFile(L"CONOUT$", 
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);


	buffer = new unsigned char[WIDTH_SCREEN * HEIGHT_SCREEN];
}

void AsciiRaster::clearscreen()
{
	WORD attrib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	for (int x = 0; x < WIDTH_SCREEN * HEIGHT_SCREEN; x++)
	{
		BackBuffer[x].Char.UnicodeChar = ' ';
		BackBuffer[x].Attributes = attrib;
	}
}

void AsciiRaster::drawpoint(int x, int y, char symb, int color)
{
	if (y >= HEIGHT_SCREEN || x >= WIDTH_SCREEN || y < 0 || x < 0) return;

	BackBuffer[y * WIDTH_SCREEN + x].Char.UnicodeChar = symb;

	//BackBuffer[pos.y * WIDTH_SCREEN + pos.x].Attributes = FColor | (BColor << 4);

	WORD attrib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	BackBuffer[y * WIDTH_SCREEN + x].Attributes = attrib;
}

void AsciiRaster::resize(unsigned int* source, int sourceWidth, int sourceHeight)
{
	int copiedColumns = 0;
	int copiedLines = 0;

	float horizontalRatio = (float)WIDTH_SCREEN / (float)sourceWidth;
	float verticalRatio = (float)HEIGHT_SCREEN / (float)sourceHeight;
	float horizontalCurrent = 1.0f;
	float verticalCurrent = 1.0f;

	for (int x = 0; x < sourceWidth; x++)
	{
		if (copiedColumns == WIDTH_SCREEN)
			break;

		if (horizontalCurrent >= 1.0f)
		{
			for (int y = 0; y < sourceHeight; y++)
			{
				if (copiedLines == HEIGHT_SCREEN)
					break;

				if (verticalCurrent >= 1.0f)
				{
					unsigned int color = source[(int)x + (int)y * sourceWidth];

					unsigned char r = UNPACK_R(color);
					unsigned char g = UNPACK_G(color);
					unsigned char b = UNPACK_B(color);
					unsigned char gray = (unsigned char)(0.299f * r + 0.587 * g + 0.114f * b);

					buffer[copiedLines * WIDTH_SCREEN + copiedColumns] = gray;

					copiedLines += 1;
					verticalCurrent -= 1.0f;
				}

				verticalCurrent += verticalRatio;
			}

			copiedLines = 0;
			copiedColumns += 1;
			horizontalCurrent -= 1.0f;
			verticalCurrent = 1.0f;
		}

		horizontalCurrent += horizontalRatio;
	}
}


void AsciiRaster::render(unsigned int* renderbuffer)
{
	resize(renderbuffer, SCREEN_XSIZE, SCREEN_YSIZE);

	for (int y = 0; y < HEIGHT_SCREEN; y++)
	{
		for (int x = 0; x < WIDTH_SCREEN; x++)
		{
			unsigned char c = buffer[y * WIDTH_SCREEN + x];
			int s = (int)((ASCII_TBL_N-1) * (1.f - (float)c / 255.f));

			char symbol = gradientTbl2[s];
			drawpoint(x, y, symbol, 0);
		}
	}

	SMALL_RECT writeArea = { 0,0, (short)WIDTH_SCREEN, (short)HEIGHT_SCREEN };
	COORD charPosition = { 0,0 };
	COORD bufferSize = { (short)WIDTH_SCREEN, (short)HEIGHT_SCREEN };

	WriteConsoleOutput(hConsole, BackBuffer, bufferSize, charPosition, &writeArea);
}

