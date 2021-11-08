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

		player = std::make_unique<Player>("Assets/monochrome.png", 510, 101, renderer, width, height, 36, 36, 8);

		// audioTriggerQueue = std::make_unique<MessageQueue<AudioTrigger>>();

		// panel = std::make_unique<Panel>(renderer, "Assets/Sprites/Title.png", width, height, 512, 256);

		controller = std::make_unique<Controller>(player.get());

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

	//// start controller and ball
	threads.emplace_back(std::thread(&Controller::getInputs, std::ref(*controller)));
	//threads.emplace_back(std::thread(&Ball::move, std::ref(*ball)));

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

		if (!isRunning /*end game condition*/)
		{
			render();

			/*panel->loadTexture("Assets/Sprites/END_PANEL.png");
			panel->render();*/
			SDL_RenderPresent(renderer);

			/*audio->stop();
			controller->stop();*/

			// audio->playSound("Assets/Audio/Panel.wav", 2000);

			isRunning = false;

			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}
	}

	for (auto& t : threads)
		t.join();

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
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	player->render();

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