#pragma once

#include "Pawn.h"

enum class Direction
{
	Left,
	Right,
	Stop
};

class Player
	: public Pawn
{
public:
	Player(const char* textureFile, int srcX, int srcY, SDL_Renderer* r, int winW, int winH, int w, int h, int vel);

	// virtual void render() override;

	virtual void move() override;
	void setDirection(Direction d) { direction = d; }
	
	// void shoot();

private:
	Direction direction = Direction::Stop;
};

typedef std::unique_ptr<Player> PlayerPtr;