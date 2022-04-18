#include <thread>
#include <iostream>

#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, 
	int winW, int winH, int w, int h, int vel, int xOffset, int yOffset, Player* p)
	: Pawn(r, t, srcX, srcY, winW, winH, w, h, vel), player(p)
{
	hitPoints = 2;

	dstRect.x = Pawn::size * 1.2 + xOffset;
	dstRect.y = Pawn::size * 1.2 + yOffset;

	std::cout << "Init enemy x, y " << dstRect.x << ", " << dstRect.y << std::endl;

	startShootingThread();
}

void Enemy::move()
{
	if (this != nullptr)
	{
		// move on x axis
		dstRect.x += velocity;
		for (auto& d : damages)
		{
			d.first += velocity;
		}

		// if window border is reached, flip down and revert direction
		if (dstRect.x >= windowWidth - Pawn::size || dstRect.x <= Pawn::size)
		{
			dstRect.y += Pawn::size;
			for (auto& d : damages)
				d.second += Pawn::size;

			velocity *= -1;
		}

		moveProjectiles();

		for (auto& p : player->getProjectiles())
		{
			if (p != nullptr)
			{
				if (checkCollision(p.get()))
				{
					auto local = std::move(p);
					local.reset();
					player->getProjectiles().erase(std::remove(player->getProjectiles().begin(), player->getProjectiles().end(), p));
				}
			}
		}
	}
}

void Enemy::shoot()
{
	// std::cout << this << " started thread: shoot()\n";

	int tempTrigger = 0;

	while (isRunning)
	{	
		setShooting();

		tempTrigger = shootingDelayDistr(mt);

		if (tempTrigger == 1 && isShooting)
			addProjectile(Direction::Down);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));		
	}

	// std::cout << this << " stopped thread: shoot()\n";
}

void Enemy::setShooting()
{
	isShooting = true;
	for (auto* e : enemies)
	{
		if (e != nullptr)
		{
			if (dstRect.x == e->dstRect.x && dstRect.y < e->dstRect.y)
				isShooting = false;
		}
	}
}

bool Enemy::destroyed()
{
	if (this != nullptr)
	{
		if (hitPoints == 0)
		{
			stop();
			enemies.clear();
			damages.clear();
			for (auto& p : projectiles)
			{
				auto local = std::move(p);
				local.reset();
			}

			t.join();
			return true;
		}

		return false;
	}
}