#pragma once
#include <string>
#include <SDL.h>
#include <map>

// Forward decares
struct SDL_Window;
struct SDL_Renderer;

// Class
class Graphics {
public:
	Graphics(int win_x, int win_y, std::string name);
	~Graphics();

	SDL_Surface* loadImage(const std::string& filePath);

	void clear();
	void blitSurface(SDL_Surface* texture, SDL_Rect* desitnationRectangle);
	void flip();
	SDL_Surface* getSurface() const;
	

private:
	SDL_Window* _window = NULL;
	SDL_Surface* _screenSurface = NULL;
};