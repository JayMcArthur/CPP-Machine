#pragma once

#include <SDL.h>
#include <map>

class Input
{
public:
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& events);
	void keyDownEvent(const SDL_Event& events);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);
private:
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;
};

