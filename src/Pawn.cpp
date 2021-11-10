#include <iostream>

#include "Pawn.h"

Pawn::Pawn(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel)
	: renderer(r), GameObject(r, winW, winH, w, h, vel)
{
	loadTexture("Assets/monochrome.png");

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = 18;
	srcRect.h = 18;
}

void Pawn::render()
{
	std::lock_guard<std::mutex> lck(mtx);
	SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
	for (auto& p : projectiles)
	{
		if (p != nullptr)
			p->render();
	}
}

void Pawn::loadTexture(const char* file)
{
	std::lock_guard<std::mutex> lck(mtx);
	SDL_Surface* tempSurface = IMG_Load(file);
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void Pawn::addProjectile(Direction dir)
{
	std::lock_guard<std::mutex> lck(mtx);
	projectiles.emplace_back(std::make_unique<Projectile>(dstRect, dir, renderer, 2, 2, 8));
}

void Pawn::moveProjectiles()
{
	std::lock_guard<std::mutex> lck(mtx);
	for (auto& p : projectiles)
	{
		if (p != nullptr)
		{
			p->move(); 
			if (p->dstRect.y <= 0 || p->dstRect.y > windowHeight)
				p.reset();
		}		
	}	
}