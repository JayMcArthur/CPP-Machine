#include <SDL_image.h>
#include "graphics.h"

// Makes a game window and names it
Graphics::Graphics(int win_x, int win_y, std::string name)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Machine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_x, win_y, SDL_WINDOW_SHOWN);
	SDL_SetWindowTitle(_window, name.c_str());
	_screenSurface = SDL_GetWindowSurface(_window);
}

// Kills the game window
Graphics::~Graphics()
{
	SDL_FreeSurface(_screenSurface);
	_screenSurface = NULL;
	SDL_DestroyWindow(_window);
	_window = NULL;
}

SDL_Surface* Graphics::loadImage(const std::string& filePath)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	// Load Image
	SDL_Surface* temp = IMG_Load(filePath.c_str());

	//Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface(temp, _screenSurface->format, 0);

	//Get rid of old loaded surface
	SDL_FreeSurface(temp);

	return optimizedSurface;
}

void Graphics::clear()
{
	SDL_FillRect(_screenSurface, NULL, SDL_MapRGB(_screenSurface->format, 0xFF, 0xFF, 0xFF));
}

void Graphics::blitSurface(SDL_Surface* texture, SDL_Rect* desitnationRectangle)
{
	SDL_BlitScaled(texture, NULL, _screenSurface, desitnationRectangle);
}

void Graphics::flip()
{
	SDL_UpdateWindowSurface(_window);
}

SDL_Surface* Graphics::getSurface() const
{
	return _screenSurface;
}