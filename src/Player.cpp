#include "Player.h"

Player::Player(const char* textureFile, int srcX, int srcY, SDL_Renderer* r, int winW, int winH, int w, int h, int vel)
	: Pawn(textureFile, srcX, srcY, r, winW, winH, w, h, vel)
{
	dstRect.x = winW / 2 - 18 / 2;
	dstRect.y = winH / 2 - 18 / 2;
}

void Player::move()
{
	dstRect.x += getVelocity();
}