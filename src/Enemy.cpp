#include <thread>
#include <iostream>

#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel, int& xOffset, int& yOffset, Player* p)
	: Pawn(r, srcX, srcY, winW, winH, w, h, vel), player(p)
{
	dstRect.x = defaultX + xOffset;
	dstRect.y = defaultY + yOffset;

	startShootingThread();
}

void Enemy::move()
{
	if (this != nullptr)
	{
		// std::cout << this << " " << isRunning << std::endl;
		dstRect.x += velocity;
		for (auto& d : damages)
		{
			d.first += velocity;
			/*d.second += velocity;*/
		}

		moveProjectiles();

		if (dstRect.x >= windowWidth - Pawn::size || dstRect.x <= Pawn::size)
		{
			dstRect.y += Pawn::size;
			for (auto& d : damages)
				d.second += Pawn::size;

			velocity *= -1;
		}

		for (auto& p : player->getProjectiles())
		{
			if (p != nullptr)
			{
				if (checkCollision(p.get()))
				{
					auto local = std::move(p);
					local.reset();
					/*player->projectiles.erase(std::remove(player->projectiles.begin(), player->projectiles.end(), p));*/
				}
			}
		}
	}
}

void Enemy::shoot()
{
	// std::cout << this << " started thread: shoot()\n";

	int tempCounter = 0;

	int tempTrigger = 0;

	while (isRunning)
	{	
		setShooting();

		tempTrigger = shootingDelayDistr(mt);

		if (tempTrigger == 1 && isShooting)
		{
			addProjectile(Direction::Down);
		}

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
			{
				isShooting = false;
			}
		}
	}
}