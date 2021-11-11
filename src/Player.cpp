#include <iostream>
#include <cassert>

#include "Player.h"

Player::Player(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel, MessageQueue<ShootingTrigger>* sT)
	: Pawn(r, srcX, srcY, winW, winH, w, h, vel)
{
	shootingTriggerQueue = sT;
	dstRect.x = winW / 2 - 18 / 2;
	dstRect.y = winH - winH / 12;
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

	moveProjectiles();
}

void Player::shoot()
{
	std::cout << "Started thread: shoot()\n";

	ShootingTrigger shootingTrigger = ShootingTrigger::DontShoot;

	while (isRunning)
	{
		shootingTrigger = ShootingTrigger::DontShoot;
		shootingTrigger = shootingTriggerQueue->receive();

		if (shootingTrigger == ShootingTrigger::Shoot)
		{
			addProjectile(Direction::Up);
			
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		} 
			
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
	}

	std::cout << "Stopped thread: shoot()\n";
}

void Player::setDirection(Direction d)
{
	// assert(d == Direction::Right || d == Direction::Left);
	direction = d; 
}