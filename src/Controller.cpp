#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

#include "Controller.h"

Controller::Controller(Player* p)
	: player(p)
{}

void Controller::getInputs()
{
	assert(player != nullptr);

	std::cout << "Started thread: Controller::getInputs()\n";

	const uint8_t* state = SDL_GetKeyboardState(NULL);

	while (isRunning)
	{
		SDL_PumpEvents();

		if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
			player->setDirection(Direction::Left);

		else if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A])
			player->setDirection(Direction::Right);

		else
			player->setDirection(Direction::Stop);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::cout << "Stopped thread: Controller::getInputs()\n";
}