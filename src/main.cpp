#include <memory>

#include "Game.h"



int main(int argc, char* argv[])
{
	constexpr int windowSize[2] = { 800, 600 };

	Game game("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize[0], windowSize[1], false);
	game.run();

	return 0;
}