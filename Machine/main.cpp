#include <SDL_image.h>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <string>
#include <SDL.h>
#include <map>

#include "graphics.h"
#include "input.h"


const int WIN_WIDTH = 960;
const int WIN_HEIGHT = 540;
const int TILE_SIZE = 30;

bool record = false;
bool group = false;
int playInedx = 0;

float _cameraX = 0;
float _X_Old = _cameraX;
float _cameraY = 0;
float _Y_Old = _cameraY;


/*
	MAKE WINDOW
*/
Graphics* window = new Graphics(WIN_WIDTH, WIN_HEIGHT, "Machine");

/*
	MAKE Images
*/
SDL_Surface* tile_bg = window->loadImage("Sprites/Background.png");
SDL_Surface* tile_up = window->loadImage("Sprites/Up.png");
SDL_Surface* tile_right = window->loadImage("Sprites/Right.png");
SDL_Surface* tile_down = window->loadImage("Sprites/Down.png");
SDL_Surface* tile_left = window->loadImage("Sprites/Left.png");
SDL_Surface* tile_horiz = window->loadImage("Sprites/Horizontal_Part.png");
SDL_Surface* tile_vert = window->loadImage("Sprites/Vertical_Part.png");
SDL_Surface* tile_dot = window->loadImage("Sprites/Dot.png");
SDL_Surface* tile_Spawner = window->loadImage("Sprites/Spawner.png");


struct data
{
	SDL_Surface* image;
	bool Spawner = false;
};

std::map<int, std::map<int, data>> level;
std::map<int, std::map<int, data>> levelSave;

std::vector<std::vector<float>> action;
SDL_Surface* currentItem = tile_up;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct Tile
{
public:
	int _posX;
	int _posY;
	SDL_Surface* texture;
	Tile(int x, int y, SDL_Surface* image)
	{
		_posX = x;
		_posY = y;
		texture = image;
	}
	void Check()
	{
		if (_posX > WIN_WIDTH )
		{
			_posX -= WIN_WIDTH + TILE_SIZE;
		}
		else if (_posX < 0 - TILE_SIZE)
		{
			_posX += WIN_WIDTH + TILE_SIZE;
		}

		if (_posY > WIN_HEIGHT)
		{
			_posY -= WIN_HEIGHT + TILE_SIZE;
		}
		else if (_posY < 0 - TILE_SIZE)
		{
			_posY += WIN_HEIGHT + TILE_SIZE;
		}

		updateBackground();

	}

	void updateBackground()
	{
		int relativeX = (_posX - _cameraX) / TILE_SIZE;
		int relativeY = (_posY - _cameraY) / TILE_SIZE;
		if (level.find(relativeX) != level.end() && level[relativeX].find(relativeY) != level[relativeX].end())
		{
			if (level[relativeX][relativeY].image == tile_Spawner)
			{
				level[relativeX][relativeY].image = tile_dot;
			}
			else
			{
				texture = level[relativeX][relativeY].image;
			}
		}
		else
		{
			texture = tile_bg;
		}
		

	}

	void Move(SDL_Surface* image, int rX, int rY, int x, int y)
	{
		if (level.find(rX + x) != level.end() && level[rX + x].find(rY + y) != level[rX + x].end() && level[rX + x][rY + y].image != tile_Spawner)
		{
			Move(level[rX + x][rY + y].image, rX, rY, x + sgn(x), y + sgn(y));
		}
		level[rX + x][rY + y].image = image;
	}

	void handleEvent(SDL_Event* e)
	{
		if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (x < _posX)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (x > _posX + TILE_SIZE)
			{
				inside = false;
			}
			//Mouse above the button
			else if (y < _posY)
			{
				inside = false;
			}
			//Mouse below the button
			else if (y > _posY + TILE_SIZE)
			{
				inside = false;
			}

			//Mouse is inside button
			else
			{
				if (record && (texture == tile_up || texture == tile_right || texture == tile_down || texture == tile_left))
				{
					// Save actions
					std::vector<float> temp = { (_posX - _cameraX) / TILE_SIZE, (_posY - _cameraY) / TILE_SIZE };
					action.push_back(temp);

					int rX = (_posX - _cameraX) / TILE_SIZE;
					int rY = (_posY - _cameraY) / TILE_SIZE;

					// Preform Action
					if (texture == tile_up)
					{
						Move(texture, rX, rY, 0, -1);
					}
					else if (texture == tile_right)
					{
						Move(texture, rX, rY, 1, 0);
					}
					else if (texture == tile_down)
					{
						Move(texture, rX, rY, 0, 1);
					}
					else if (texture == tile_left)
					{
						Move(texture, rX, rY, -1, 0);
					}

					if (level[rX][rY].Spawner)
					{
						level[(_posX - _cameraX) / TILE_SIZE][(_posY - _cameraY) / TILE_SIZE].image = tile_Spawner;
					}
					else
					{
						level[(_posX - _cameraX) / TILE_SIZE].erase((_posY - _cameraY) / TILE_SIZE);
						if (level[(_posX - _cameraX) / TILE_SIZE].empty())
						{
							level.erase((_posX - _cameraX) / TILE_SIZE);
						}
					}
					
					
				}
				else if (record == false)
				{
					if (texture != currentItem)
					{
						texture = currentItem;
						level[(_posX - _cameraX) / TILE_SIZE][(_posY - _cameraY) / TILE_SIZE].image = currentItem;
						if (currentItem == tile_Spawner)
						{
							level[(_posX - _cameraX) / TILE_SIZE][(_posY - _cameraY) / TILE_SIZE].Spawner = true;
						}
						else
						{
							level[(_posX - _cameraX) / TILE_SIZE][(_posY - _cameraY) / TILE_SIZE].Spawner = false;
						}
					}
					else
					{
						texture = tile_bg;
						level[(_posX - _cameraX) / TILE_SIZE].erase((_posY - _cameraY) / TILE_SIZE);
						if (level[(_posX - _cameraX) / TILE_SIZE].empty())
						{
							level.erase((_posX - _cameraX) / TILE_SIZE);
						}
					}
				}
			}
		}
	}
};

std::vector<Tile> TILES;



int main(int argc, char* argv[])
{
	/*
		MAKES BACKGROUND
	*/
	for (int i = 0; i * TILE_SIZE < WIN_WIDTH + TILE_SIZE; i++)
	{
		for (int j = 0; j * TILE_SIZE < WIN_HEIGHT + TILE_SIZE; j++)
		{
			Tile* temp;
			if (level.find(i) != level.end() && level[i].find(j) != level[i].end())
			{
				temp = new Tile(i * TILE_SIZE, j * TILE_SIZE, level[i][j].image);
			}
			else
			{
				temp = new Tile(i * TILE_SIZE, j * TILE_SIZE, tile_bg);
			}
			TILES.push_back(*temp);
		}
	}

	/*
		GAME LOOP
	*/
	SDL_Event events;
	Input input;

	while (true) {
		input.beginNewFrame();

		// Check if there are events and add them to stack
		if (SDL_PollEvent(&events))
		{
			// Gets Key presses
			if (events.type == SDL_KEYDOWN)
			{
				if (events.key.repeat == 0)
				{
					input.keyDownEvent(events);
				}
			}
			else if (events.type == SDL_KEYUP)
			{
				input.keyUpEvent(events);
			}

			// Quit if pressed red X
			if (events.type == SDL_QUIT)
			{
				break;
			}

			//Handle button events
			for (int i = 0; i < TILES.size(); ++i)
			{
				TILES[i].handleEvent(&events);
			}
		}

		// Quit if pressed escape
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
		{
			break;
		}

		// Move Right and left
		if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true && input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true)
		{
			_cameraX -= 2;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_LEFT) == true)
		{
			_cameraX -= TILE_SIZE / 2;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true && input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true)
		{
			_cameraX += 2;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true)
		{
			_cameraX += TILE_SIZE / 2;
		}
		

		// Move Up and down
		if (input.isKeyHeld(SDL_SCANCODE_UP) == true && input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true)
		{
			_cameraY -= 2;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_UP) == true)
		{
			_cameraY -= TILE_SIZE / 2;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true && input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true)
		{
			_cameraY += 2;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true)
		{
			_cameraY += TILE_SIZE / 2;
		}

	
		// CHanges Item
		if (input.wasKeyPressed(SDL_SCANCODE_X) == true)
		{
			currentItem = tile_Spawner;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_Q) == true)
		{
			currentItem = tile_horiz;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_E) == true)
		{
			currentItem = tile_vert;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_W) == true)
		{
			currentItem = tile_up;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_D) == true)
		{
			currentItem = tile_right;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_S) == true)
		{
			currentItem = tile_down;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_A) == true)
		{
			currentItem = tile_left;
		}

		// Handles auto play and grouping
		if (input.wasKeyPressed(SDL_SCANCODE_P) == true)
		{
			record = false;
			group = false;

			// Preform Action
			if (level[action[playInedx][0]][action[playInedx][1]].image == tile_up)
			{
				TILES[0].Move(level[action[playInedx][0]][action[playInedx][1]].image, action[playInedx][0], action[playInedx][1], 0, -1);
			}
			else if (level[action[playInedx][0]][action[playInedx][1]].image == tile_right)
			{
				TILES[0].Move(level[action[playInedx][0]][action[playInedx][1]].image, action[playInedx][0], action[playInedx][1], 1, 0);
			}
			else if (level[action[playInedx][0]][action[playInedx][1]].image == tile_down)
			{
				TILES[0].Move(level[action[playInedx][0]][action[playInedx][1]].image, action[playInedx][0], action[playInedx][1], 0, 1);
			}
			else if (level[action[playInedx][0]][action[playInedx][1]].image == tile_left)
			{
				TILES[0].Move(level[action[playInedx][0]][action[playInedx][1]].image, action[playInedx][0], action[playInedx][1], -1, 0);
			}

			if (level[action[playInedx][0]][action[playInedx][1]].Spawner)
			{
				level[action[playInedx][0]][action[playInedx][1]].image = tile_Spawner;
			}
			else
			{
				level[action[playInedx][0]].erase(action[playInedx][1]);
				if (level[action[playInedx][0]].empty())
				{
					level.erase(action[playInedx][0]);
				}
			}
			
			++playInedx %= action.size();
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_O) == true)
		{
			playInedx = 0;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_R) == true)
		{
			record ^= 1;
			if (record)
			{
				levelSave = level;
			}
			else
			{
				level = levelSave;
			}
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_T) == true)
		{
			action.clear();
		}
		else if (record == true && input.wasKeyPressed(SDL_SCANCODE_G) == true)
		{
			group ^= 1;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_M) == true)
		{
			levelSave = level;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_N) == true)
		{
			level = levelSave;
		}


		/*
			DRAW EVERYTHING
		*/
		window->clear();

		SDL_Rect destinationRectangle;
		for (int i = 0; i < TILES.size(); i++)
		{
			TILES[i]._posX += _cameraX - _X_Old;
			TILES[i]._posY += _cameraY - _Y_Old;
			TILES[i].Check();
			destinationRectangle.x = TILES[i]._posX;
			destinationRectangle.y = TILES[i]._posY;
			destinationRectangle.w = TILE_SIZE;
			destinationRectangle.h = TILE_SIZE;
			window->blitSurface(TILES[i].texture, &destinationRectangle);
		}

		_X_Old = _cameraX;
		_Y_Old = _cameraY;
		window->flip();
	}


	/*
		CLOSE EVERYTHING
	*/
	delete(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
	return 0;
}