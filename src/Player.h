#pragma once

#include "Pawn.h"
#include "MessageQueue.h"

class Player
	: public Pawn
{
public:
	Player(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel, MessageQueue<ShootingTrigger>* sT);

	virtual void move() override;
	void setDirection(Direction d);
	
	virtual void shoot() override;
	
private:
	Direction direction = Direction::Stop;
	MessageQueue<ShootingTrigger>* shootingTriggerQueue;
};

typedef std::unique_ptr<Player> PlayerPtr;