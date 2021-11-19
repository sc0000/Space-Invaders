#include "Panel.h"

Panel::Panel(SDL_Renderer* r, SDL_Texture* t, int winW, int winH, int w, int h)
	: GameObject(r, winW, winH, w, h), texture(t)
{
	dstRect.x = winW / 2 - w / 2;
	dstRect.y = winH / 2 - h / 2;
}

void Panel::render()
{
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}
