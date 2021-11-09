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

void Projectile::render()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> distr(-8, 8);

	if (hasCollided == false)
	{
		SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);
		SDL_RenderFillRect(getRenderer(), &dstRect);
	}

	else if (hasCollided == true)
	{
		SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 0);
		SDL_RenderFillRect(getRenderer(), &dstRect);

		for (int i = 0; i < 24; ++i)
		{
			SDL_RenderDrawPoint(getRenderer(), dstRect.x + distr(mt), dstRect.y + distr(mt));
		}
	}
}

void Projectile::move()
{
	if (direction == Direction::Down)
		dstRect.y += getVelocity();

	if (direction == Direction::Up)
		dstRect.y -= getVelocity();
}

// TODO: Projectiles hold their own pointers to GameObject vector; do collision checks on all of them
void Projectile::collision(GameObject* other)
{
	SDL_Rect projectileRect = dstRect;
	SDL_Rect otherRect = other->dstRect;

	if (projectileRect.x + projectileRect.w >= otherRect.x &&
		otherRect.x + otherRect.w >= projectileRect.x &&
		projectileRect.y + projectileRect.h >= otherRect.y &&
		otherRect.y + otherRect.h >= projectileRect.y)
	{
		hasCollided = true;
		setVelocity(0);
	}
}