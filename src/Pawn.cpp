#include "Pawn.h"

Pawn::Pawn(const char* textureFile, int srcX, int srcY, SDL_Renderer* r, int winW, int winH, int w, int h, int vel)
	: renderer(r), GameObject(r, winW, winH, w, h, vel)
{
	loadTexture(textureFile);

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = 18;
	srcRect.h = 18;
}

void Pawn::render()
{
	std::lock_guard<std::mutex> lck(mtx);
	SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void Pawn::loadTexture(const char* file)
{
	std::lock_guard<std::mutex> lck(mtx);
	SDL_Surface* tempSurface = IMG_Load(file);
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}