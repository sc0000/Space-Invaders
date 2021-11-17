#pragma once

#include "Pawn.h"
#include "Counter.h"
#include "MessageQueue.h"

class Enemy;

class Player
	: public Pawn
{
public:
	Player(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, 
		int winW, int winH, int w, int h, int vel, 
		MessageQueue<ShootingTrigger>* sT, Counter* c);

	virtual void move() override;
	void setDirection(Direction d);
	
	virtual void shoot() override;

private:
	Direction direction = Direction::Stop;
	MessageQueue<ShootingTrigger>* shootingTriggerQueue;
	Counter* counter;
};

typedef std::unique_ptr<Player> PlayerPtr;