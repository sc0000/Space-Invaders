#include <iostream>

#include "Pawn.h"

Pawn::Pawn(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, int winW, int winH, int w, int h, int vel)
	: renderer(r), texture(t), GameObject(r, winW, winH, w, h, vel)
{
	SDL_Texture* damageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = 18;
	srcRect.h = 18;
}

void Pawn::render()
{
	if (this != nullptr)
	{
		SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

		SDL_SetRenderTarget(renderer, damageTexture);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (auto& d : damages)
		{
			SDL_RenderDrawPoint(renderer, d.first, d.second);
		}

		for (auto& p : projectiles)
		{
			if (p != nullptr)
				p->render();
		}
	}
}

void Pawn::renderDefeat()
{
	if (this != nullptr)
	{
		for (int i = 0; i < 3; ++i)
		{
			SDL_RenderClear(renderer);

			SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
			for (auto& d : damages)
			{
				SDL_RenderDrawPoint(renderer, d.first, d.second);
			}
			SDL_RenderPresent(renderer);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &dstRect);
			SDL_RenderPresent(renderer);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
}

void Pawn::addProjectile(Direction dir)
{
	std::lock_guard<std::mutex> lck(mtx);
	if (this != nullptr)
		projectiles.emplace_back(std::make_unique<Projectile>(dstRect, dir, renderer, 2, 2, 8));
}

void Pawn::moveProjectiles()
{
	std::lock_guard<std::mutex> lck(mtx);
	for (auto& p : projectiles)
	{
		if (p != nullptr && projectiles.size() != 0)
		{
			p->move(); 
			if (p->dstRect.y <= 0 || p->dstRect.y > windowHeight)
			{
				auto local = std::move(p);
				local.reset();
				projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), p));
			}
		}		
	}	
}

bool Pawn::checkCollision(Projectile* projectile)
{
	if (projectile != nullptr && this != nullptr)
	{
		if (dstRect.x + dstRect.w >= projectile->dstRect.x &&
			projectile->dstRect.x + projectile->dstRect.w >= dstRect.x &&
			dstRect.y + dstRect.h >= projectile->dstRect.y &&
			projectile->dstRect.y + projectile->dstRect.h >= dstRect.y)
		{
			if (projectile->getDirection() == Direction::Up)
			{
				for (int i = 0; i < 512; ++i)
					damages.emplace_back(projectile->dstRect.x + (destructionDistr(mt) / 2),
						projectile->dstRect.y - abs(destructionDistr(mt)));
			}
			
			else if (projectile->getDirection() == Direction::Down)
			{
				for (int i = 0; i < 128; ++i)
					damages.emplace_back(projectile->dstRect.x + (destructionDistr(mt) / 2),
						projectile->dstRect.y + abs(destructionDistr(mt)));	
			}

			std::lock_guard<std::mutex> lck(hitCounterMtx);
			hitPoints -= 1;
				
			return true;
		}

		return false;
	}
	
	return false;
}