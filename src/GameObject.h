#pragma once

#include <memory>
#include <vector>
#include <mutex>

#include "SDL.h"
#include "SDL_image.h"

enum class Direction
{
	Left,
	Right,
	Up,
	Down,
	Stop
};

class GameObject
{
public:
	// GameObject() {}
	GameObject(SDL_Renderer* r, int winW, int winH, int w, int h, int vel=0);

	virtual void render();
	virtual void move() {}

	SDL_Renderer* getRenderer() { return renderer; }
	int getVelocity() { return velocity; }
	void setVelocity(int vel) { velocity = vel; }
	std::vector<int> getWindowSize() { return { windowWidth, windowHeight }; }

	SDL_Rect dstRect;

private:
	SDL_Renderer* renderer;

	int windowWidth, windowHeight;
	int velocity;
};

