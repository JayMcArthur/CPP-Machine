#include "input.h"

void Input::beginNewFrame()
{
	_pressedKeys.clear();
	_releasedKeys.clear();
}

void Input::keyUpEvent(const SDL_Event& events)
{
	_releasedKeys[events.key.keysym.scancode] = true;
	_heldKeys[events.key.keysym.scancode] = false;
}

void Input::keyDownEvent(const SDL_Event& events)
{
	_pressedKeys[events.key.keysym.scancode] = true;
	_heldKeys[events.key.keysym.scancode] = true;
}

bool Input::wasKeyPressed(SDL_Scancode key)
{
	return _pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key)
{
	return _releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Scancode key)
{
	return _heldKeys[key];
}