#pragma once

#include "Pawn.h"

class Player
	: public Pawn
{
public:
	Player(const char* textureFile, int srcX, int srcY, SDL_Renderer* r, int winW, int winH, int w, int h, int vel);

	// virtual void render() override;
	virtual void move() override;
	// void shoot();

private:
	
};

typedef std::unique_ptr<Player> PlayerPtr;