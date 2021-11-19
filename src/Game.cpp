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
		// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		isRunning = true;

		// audioTriggerQueue = std::make_unique<MessageQueue<AudioTrigger>>();
		shootingTriggerQueue = std::make_unique<MessageQueue<ShootingTrigger>>();

		// panel = std::make_unique<Panel>(renderer, "Assets/Sprites/Title.png", width, height, 512, 256);

		spriteTexture = loadTexture("Assets/Sprites/monochrome.png");
		panelTexture = loadTexture("Assets/Sprites/title.png");
		
		panel = std::make_unique<Panel>(renderer, panelTexture, width, height, 512, 256);
		counter = std::make_unique<Counter>(renderer, spriteTexture, width, height, 36, 36);
		player = std::make_unique<Player>(renderer, spriteTexture, 510, 102, width, height, 36, 36, 8, 
			shootingTriggerQueue.get(), counter.get());
		initEnemies(4, 12, width, height, 1);
		player->setEnemies(enemies);

		initShields(12, width, height);
		for (auto& s : shields)
			s->setEnemies(enemies);

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

	SDL_RenderClear(renderer);
	panel->render();
	SDL_RenderPresent(renderer);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//
	//audio->playSound("Assets/Audio/Panel.wav", 2000);

	//// start controller and shooting threads
	threads.emplace_back(std::thread(&Controller::getInputs, std::ref(*controller)));
	threads.emplace_back(std::thread(&Player::shoot, std::ref(*player)));

	//for (auto& e : enemies)
	//{
	//	e->startShootingThread();
	//	// threads.emplace_back(std::thread(&Enemy::shoot, std::ref(*e)));
	//}

	while (outcome != Outcome::Victory && outcome != Outcome::Defeat)
	{
		Uint32 frameStart = SDL_GetTicks();

		for (auto& e : enemies)
			e->setEnemies(enemies);

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

	controller->stop();
	player->stop();

	for (auto& e : enemies)
	{
		e->stop();
		e->joinShootingThread();
	}

	for (auto& t : threads)
		t.join();

	if (outcome == Outcome::Defeat)
	{
		player->renderDefeat();
		SDL_RenderClear(renderer);
		panel->setTexture(loadTexture("Assets/Sprites/defeat.png"));
		panel->render();
		SDL_RenderPresent(renderer);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	if (outcome == Outcome::Victory)
	{
		SDL_RenderClear(renderer);
		panel->setTexture(loadTexture("Assets/Sprites/victory.png"));
		panel->render();
		SDL_RenderPresent(renderer);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

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
		break;

	default:
		break;
	}
}

void Game::update()
{
	player->move();
	updateEnemies();
	for (auto& e : enemies)
	{
		if (e != nullptr)
			e->move();
	}

	updateShields();
	for (auto& s : shields)
	{
		if (s != nullptr)
			s->update();
	}

	if (counter->getCounter() <= 0)
		outcome = Outcome::Defeat;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	player->render();

	for (auto& e : enemies)
	{
		if (e != nullptr)
			e->render();
	}

	for (auto& s : shields)
	{
		if (s != nullptr)
			s->render();
	}

	counter->render();

	SDL_RenderPresent(renderer);
}

void Game::cleanup()
{
	std::cout << "Quit program..." << std::endl;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

SDL_Texture* Game::loadTexture(const char* file)
{
	SDL_Surface* tempSurface = IMG_Load(file);
	SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return temp;
}

void Game::initEnemies(int rows, int columns, int winW, int winH, int vel)
{
	int xOffset = 0;
	int yOffset = 0;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			enemies.emplace_back(std::make_unique<Enemy>(renderer, spriteTexture, 442, 51,
				winW, winH, Pawn::size, Pawn::size, vel, xOffset, yOffset, player.get()));
			xOffset += Enemy::enemyPosOffset;
		}

		xOffset = 0;
		yOffset += Enemy::enemyPosOffset;;
	}
	
	for (auto& e : enemies)
		e->setEnemies(enemies);

	std::cout << "Number of enemies: " << enemies.size() << std::endl;
}

void Game::updateEnemies()
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i]->destroyed())
		{
			// enemies[i]->stop();
			std::cout << enemies[i] << " has been destroyed\n";
			if (enemies.size() == 1 && i == 0)
			{
				enemies[0]->renderDefeat();
				outcome = Outcome::Victory;
			}

			auto local = std::move(enemies[i]);
			enemies.erase(std::remove(enemies.begin(), enemies.end(), enemies[i]));
			local.reset();

			std::cout << "Nr of enemies: " << enemies.size() << std::endl;

			for (auto& e : enemies)
				e->setEnemies(enemies);
		}
	}
}

	/*else if (enemies.size() == 1 && enemies[0]->destroyed())
	{
			enemies[0]->renderDefeat();
			outcome = Outcome::Victory;
	}*/


void Game::initShields(int number, int winW, int winH)
{
	for (int i = 0; i < number; ++i)
	{
		shields.emplace_back(std::make_unique<Shield>(renderer, spriteTexture, 152, 203, winW, winH, 36, 36, player.get()));
		shields[i]->dstRect.x = winW / (number + 1) * (i + 1);
		shields[i]->dstRect.y = (winH - winH / 12) - Pawn::size * 2.4;
	}
}

void Game::updateShields()
{
	for (size_t i = 0; i < shields.size(); ++i)
	{
		if (shields[i]->destroyed())
		{
			// shields[i]->stop();
			std::cout << shields[i] << " has been destroyed\n";
			auto local = std::move(shields[i]);
			shields.erase(std::remove(shields.begin(), shields.end(), shields[i]));
			local.reset();

			std::cout << "Nr of shields: " << shields.size() << std::endl;
		}
	}
}