#pragma once

#include <memory>

#include "SDL.h"

#include "Player.h"

class Controller
{
public:
	Controller(Player* p);

	void getInputs();

	void stop() { isRunning = false; }

private:
	Player* player;

	bool isRunning = true;
};

typedef std::unique_ptr<Controller> ControllerPtr;

