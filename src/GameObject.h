#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <random>

#include "SDL.h"
#include "SDL_image.h"

static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_int_distribution<> destructionDistr(-8, 8);
static std::uniform_int_distribution<> shootingDelayDistr(1000, 3000);

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

	//std::vector<int> getWindowSize() { return { windowWidth, windowHeight }; }

	SDL_Rect dstRect;

protected:
	SDL_Renderer* renderer;
	int velocity;
	int windowWidth, windowHeight;
};

