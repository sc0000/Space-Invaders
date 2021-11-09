#pragma once

#include <memory>

#include "SDL.h"

#include "Player.h"
#include "MessageQueue.h"

class Controller
{
public:
	Controller(Player* p, MessageQueue<ShootingTrigger>* sT);

	void getInputs();

	void stop() { isRunning = false; }

private:
	Player* player;
	MessageQueue<ShootingTrigger>* shootingTriggerQueue;

	bool isRunning = true;
};

typedef std::unique_ptr<Controller> ControllerPtr;

