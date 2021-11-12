#include "Counter.h"


Counter::Counter(SDL_Renderer* r, SDL_Texture* t, int winW, int winH, int w, int h)
	: GameObject(r, winW, winH, w, h), texture(t)
{
	dstRect.x = 2;
	dstRect.y = winH - winH / 12;

	dstRect2.x = dstRect.x + Pawn::size;
	dstRect2.y = dstRect.y;
	dstRect2.w = dstRect.w;
	dstRect2.h = dstRect.h;

	iconRect.x = 659;
	iconRect.y = 169;
	iconRect.w = 18;
	iconRect.h = 18;

	counterRect.x = 681;
	counterRect.y = 288;
	counterRect.w = 18;
	counterRect.h = 18;
}

void Counter::render()
{
	if (this != nullptr)
	{
		SDL_RenderCopy(renderer, texture, &iconRect, &dstRect);
		SDL_RenderCopy(renderer, texture, &counterRect, &dstRect2);
	}
};