#include <iostream>
#include <string>

#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* r, int winW, int winH, int w, int h, int vel)
	: renderer(r), windowWidth(winW), windowHeight(winH), velocity(vel)
{
	dstRect.w = w;
	dstRect.h = h;
	// dstRect.x, dstRect.y see child classes
}

void GameObject::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &dstRect);
}