#pragma once

#include <vector>
#include <memory>
#include <thread>

#include "SDL.h"

#include "MessageQueue.h"
#include "Player.h"
#include "Enemy.h"
#include "Controller.h"

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

	void initEnemies(int rows, int columns, int winW, int winH, int vel);

	// void addGameObject(GameObject* gO) { gameObjects.emplace_back(gO); }

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	std::unique_ptr<MessageQueue<AudioTrigger>> audioTriggerQueue;
	std::unique_ptr<MessageQueue<ShootingTrigger>> shootingTriggerQueue;

	PlayerPtr player;

	// PanelPtr panel;

	ControllerPtr controller;

	// AudioPtr audio;

	// std::vector<GameObject*> gameObjects;
	std::vector<EnemyPtr> enemies;

	std::vector<std::thread> threads;

	bool isRunning = false;
};

