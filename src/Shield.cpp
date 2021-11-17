#include "Shield.h"
#include "Enemy.h"

Shield::Shield(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, int winW, int winH, int w, int h, Player* p)
	: Pawn(r, t, srcX, srcY, winW, winH, w, h), player(p)
{
	hitPoints = 4;
}

void Shield::update()
{
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
						auto local = std::move(p);
						local.reset();
						enemy->getProjectiles().erase(std::remove(enemy->getProjectiles().begin(), enemy->getProjectiles().end(), p));
					}
				}
			}
		}

		if (player->getProjectiles().size() > 0)
		{
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
}

bool Shield::destroyed()
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

			return true;
		}

		return false;
	}
}
