#pragma once

#include "Pawn.h"
#include "MessageQueue.h"

class Player
	: public Pawn
{
public:
	Player(const char* textureFile, int srcX, int srcY, MessageQueue<ShootingTrigger>* sT, SDL_Renderer* r, int winW, int winH, int w, int h, int vel);

	virtual void move() override;
	void setDirection(Direction d) { direction = d; }
	
	void shoot();
	
	void stop() { isRunning = false; }

private:
	Direction direction = Direction::Stop;
	MessageQueue<ShootingTrigger>* shootingTriggerQueue;

	bool isRunning = true;
};

typedef std::unique_ptr<Player> PlayerPtr;