#include <iostream>

#include "Player.h"

Player::Player(const char* textureFile, int srcX, int srcY, MessageQueue<ShootingTrigger>* sT, 
	SDL_Renderer* r, int winW, int winH, int w, int h, int vel)
	: Pawn(textureFile, srcX, srcY, r, winW, winH, w, h, vel)
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

	if (dstRect.x + dstRect.w >= getWindowSize()[0])
		direction = Direction::Left;

	// move; direction is set via Controller class
	if (direction == Direction::Left)
		dstRect.x -= getVelocity();

	else if (direction == Direction::Right)
		dstRect.x += getVelocity();

	moveProjectiles();
}

void Player::shoot()
{
	std::cout << "Started thread: shoot()\n";

	ShootingTrigger shootingTrigger = ShootingTrigger::DontShoot;

	while (isRunning)
	{
		shootingTrigger = shootingTriggerQueue->receive();

		if (shootingTrigger == ShootingTrigger::Shoot)
			addProjectile(Direction::Up);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::cout << "Stopped thread: shoot()\n";
}