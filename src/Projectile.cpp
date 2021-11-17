#include <cassert>

#include "Projectile.h"


Projectile::Projectile(SDL_Rect ownerRect, Direction d, SDL_Renderer* r, int w, int h, int vel)
	: direction(d), GameObject(r, 0, 0, w, h, vel)
{
	assert(direction != Direction::Left);
	assert(direction != Direction::Right);
	assert(direction != Direction::Stop);

	dstRect.x = ownerRect.x + ownerRect.w / 2;

	if (direction == Direction::Down)
		dstRect.y = ownerRect.y + ownerRect.h + 4;

	if (direction == Direction::Up)
		dstRect.y = ownerRect.y - 4;

	/*dstRect.w = 2;
	dstRect.h = 2;*/
}

void Projectile::move()
{
	if (direction == Direction::Down)
		dstRect.y += velocity;

	if (direction == Direction::Up)
		dstRect.y -= velocity;
}