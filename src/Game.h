#pragma once

#include <vector>
#include <memory>
#include <thread>

#include "SDL.h"

#include "MessageQueue.h"
#include "Player.h"
#include "Enemy.h"
#include "Shield.h"
#include "Controller.h"
#include "Counter.h"
#include "Panel.h"

enum class Outcome
{
	Victory,
	Defeat,
	None
};

class Game
{
public:
	Game(const char* title, int xPos, int yPos, int width, int height, bool fullScreen);

	void run();

private:
	void handleEvents();
	void update();
	void render();
	void cleanup();
	
	SDL_Texture* loadTexture(const char* file);
	void initEnemies(int rows, int columns, int winW, int winH, int vel);
	void initShields(int number, int winW, int winH);
	void updateEnemies();
	void updateShields();

	// void addGameObject(GameObject* gO) { gameObjects.emplace_back(gO); }

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* spriteTexture;
	SDL_Texture* panelTexture;

	// std::unique_ptr<MessageQueue<AudioTrigger>> audioTriggerQueue;
	std::unique_ptr<MessageQueue<ShootingTrigger>> shootingTriggerQueue;

	PanelPtr panel;
	// AudioPtr audio;
	PlayerPtr player;
	CounterPtr counter;

	// std::vector<GameObject*> gameObjects;
	std::vector<EnemyPtr> enemies;
	std::vector<ShieldPtr> shields;
	ControllerPtr controller;


	std::vector<std::thread> threads;

	bool isRunning = false;
	Outcome outcome = Outcome::None;

	std::mutex mtx;
};

