#pragma once

#include <memory>
#include <vector>
#include <mutex>

#include "SDL.h"
#include "SDL_image.h"

class GameObject
{
public:
	// GameObject() {}
	GameObject(SDL_Renderer* r, int winW, int winH, int w, int h, int vel=0);

	virtual void render();
	virtual void move() {}

	int getVelocity() { return velocity; }
	std::vector<int> getWindowSize() { return { windowWidth, windowHeight }; }

	SDL_Rect dstRect;

private:
	SDL_Renderer* renderer;

	int windowWidth, windowHeight;
	int velocity;
};

