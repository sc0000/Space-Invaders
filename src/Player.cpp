#include <iostream>
#include <cassert>

#include "Player.h"
#include "Enemy.h"

Player::Player(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, 
	int winW, int winH, int w, int h, int vel, 
	MessageQueue<ShootingTrigger>* sT, Counter* c)
	: Pawn(r, t, srcX, srcY, winW, winH, w, h, vel), counter(c)
{
	hitPoints = 3;

	shootingTriggerQueue = sT;
	dstRect.x = winW / 2 - 18 / 2;
	dstRect.y = (winH - winH / 12) - Pawn::size * 1.2;
}

void Player::move()
{
	// limit movement to window borders
	if (dstRect.x <= 0)
		direction = Direction::Right;

	if (dstRect.x + dstRect.w >= windowWidth)
		direction = Direction::Left;

	// move; direction is set via Controller class
	if (direction == Direction::Left)
		dstRect.x -= velocity;

	else if (direction == Direction::Right)
		dstRect.x += velocity;

	for (auto& d : damages)
	{
		if (direction == Direction::Left)
			d.first -= velocity;

		else if (direction == Direction::Right)
			d.first += velocity;
	}

	moveProjectiles();

	for (auto* enemy : enemies)
	{
		if (enemy->getProjectiles().size() > 0)
		{
			for (auto& p : enemy->getProjectiles())
			{
				if (p != nullptr)
				{
					if (checkCollision(p.get()))
					{
						counter->update();
						auto local = std::move(p);
						local.reset();
						enemy->getProjectiles().erase(std::remove(enemy->getProjectiles().begin(), enemy->getProjectiles().end(), p));
					}
				}
			}
		}
	}
}

void Player::shoot()
{
	std::cout << "Started thread: shoot()\n";

	ShootingTrigger shootingTrigger = ShootingTrigger::DontShoot;

	while (isRunning)
	{
		shootingTrigger = shootingTriggerQueue->receive();

		if (shootingTrigger == ShootingTrigger::Shoot)
		{
			if (projectiles.size() > 4)
				projectiles.clear();
			addProjectile(Direction::Up);
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));	
	}

	std::cout << "Stopped thread: shoot()\n";
}

void Player::setDirection(Direction d)
{
	assert(d != Direction::Up);
	assert(d != Direction::Down);
	direction = d; 
}