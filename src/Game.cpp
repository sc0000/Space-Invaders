#include <iostream>

#include "Game.h"

Game::Game(const char* title, int xPos, int yPos, int width, int height, bool fullScreen)
{
	int flags = 0;
	if (fullScreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized!" << std::endl;

		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags /* || SDL_WINDOW_INPUT_GRABBED */);
		if (window)
			std::cout << "Window created!" << std::endl;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // set to white
		std::cout << "Renderer created!" << std::endl;

		isRunning = true;

		// audioTriggerQueue = std::make_unique<MessageQueue<AudioTrigger>>();
		shootingTriggerQueue = std::make_unique<MessageQueue<ShootingTrigger>>();

		// panel = std::make_unique<Panel>(renderer, "Assets/Sprites/Title.png", width, height, 512, 256);

		player = std::make_unique<Player>(renderer, 510, 102, width, height, 36, 36, 8, shootingTriggerQueue.get());

		initEnemies(4, 12, width, height, 1);

		controller = std::make_unique<Controller>(player.get(), shootingTriggerQueue.get());

		// audio = std::make_unique<Audio>(audioTriggerQueue.get());
	}

	else
		isRunning = false;
}

void Game::run()
{
	// variables for limiting frame rate
	constexpr int fps = 60;
	constexpr int frameDelay = 1000 / fps;
	Uint32 frameStart = 0; 
	int frameTime;

	//threads.emplace_back(std::thread(&Audio::run, std::ref(*audio)));

	//SDL_RenderClear(renderer);
	//panel->render();
	//SDL_RenderPresent(renderer);
	//
	//audio->playSound("Assets/Audio/Panel.wav", 2000);

	//// start controller and shooting threads
	threads.emplace_back(std::thread(&Controller::getInputs, std::ref(*controller)));
	threads.emplace_back(std::thread(&Player::shoot, std::ref(*player)));

	for (auto& e : enemies)
	{
		threads.emplace_back(std::thread(&Enemy::shoot, std::ref(*e)));
		/*e->setEnemies(enemies);
		e->setShooting();*/
	}

	while (isRunning)
	{
		Uint32 frameStart = SDL_GetTicks();

		handleEvents();
		update();
		render();

		// limiting framerate
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);

		//if (!isRunning /*end game condition*/)
		//{
		//	render();

		//	/*panel->loadTexture("Assets/Sprites/END_PANEL.png");
		//	panel->render();*/
		//	SDL_RenderPresent(renderer);

		//	/*audio->stop();
		//	controller->stop();*/

		//	// audio->playSound("Assets/Audio/Panel.wav", 2000);


		//	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		//}
	}

	for (auto& t : threads)
		t.join();

	player->stop();
	for (auto& e : enemies)
		e->stop();

	cleanup();
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT: // when the "X" button is pressed
		isRunning = false;
		/*audio->stop();
		controller->stop();*/
		

		// cleanup();
		break;

	default:
		break;

	}
}

void Game::update()
{
	player->move();

	for (auto& e : enemies)
		e->move();
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	player->render();

	for (auto& e : enemies)
		e->render();

	/*for (auto* g : gameObjects)
		g->render();*/

	SDL_RenderPresent(renderer);
}

void Game::cleanup()
{
	std::cout << "Quit program..." << std::endl;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::initEnemies(int rows, int columns, int winW, int winH, int vel)
{
	int xOffset = 0;
	int yOffset = 0;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			enemies.emplace_back(std::make_unique<Enemy>(renderer, 442, 51, winW, winH, Pawn::size, Pawn::size, vel, xOffset, yOffset));
			xOffset += Enemy::enemyPosOffset;
		}

		xOffset = 0;
		yOffset += Enemy::enemyPosOffset;
	}
	
	for (auto& e : enemies)
	{
		e->setEnemies(enemies);
		// e->setShooting();
	}
	
}