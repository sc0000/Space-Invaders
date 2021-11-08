#include "Player.h"

Player::Player(const char* textureFile, int srcX, int srcY, SDL_Renderer* r, int winW, int winH, int w, int h, int vel)
	: Pawn(textureFile, srcX, srcY, r, winW, winH, w, h, vel)
{
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
}
