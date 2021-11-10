#include <thread>
#include <iostream>

#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel, int& xOffset, int& yOffset)
	: Pawn(r, srcX, srcY, winW, winH, w, h, vel)
{
	dstRect.x = defaultX + xOffset;
	dstRect.y = defaultY + yOffset;

	// std::thread() shoot
}

void Enemy::move()
{
	dstRect.x += velocity;
	moveProjectiles();

	if (dstRect.x >= windowWidth - Pawn::size || dstRect.x <= Pawn::size)
	{
		dstRect.y += Pawn::size;
		velocity *= -1;
		// setShooting();
	}
}

void Enemy::shoot()
{
	std::cout << this << " started thread: shoot()\n";

	while (isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(shootingDelayDistr(mt)));
		setShooting();

		if (isShooting)
		{
			addProjectile(Direction::Down);
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::cout << this << " stopped thread: shoot()\n";
}

void Enemy::setShooting()
{
	isShooting = true;
	for (auto* e : enemies)
	{
		if (dstRect.x == e->dstRect.x && dstRect.y < e->dstRect.y)
		{
			isShooting = false;
		}
	}
}