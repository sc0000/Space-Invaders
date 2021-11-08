#pragma once

#include <vector>
#include <memory>
#include <thread>

#include "SDL.h"

#include "Player.h"
#include "Controller.h"

class Game
{
public:
	Game(const char* title, int xPos, int yPos, int width, int height, bool fullScreen);

	void run();
	void handleEvents();
	void update();
	void render();
	void cleanup();

	SDL_Renderer* getRenderer() { return renderer; }

	// void addGameObject(GameObject* gO) { gameObjects.emplace_back(gO); }

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	// std::unique_ptr<MessageQueue<AudioTrigger>> audioTriggerQueue;

	PlayerPtr player;

	// PanelPtr panel;

	ControllerPtr controller;

	// AudioPtr audio;

	

	// std::vector<GameObject*> gameObjects;

	std::vector<std::thread> threads;

	bool isRunning = false;
};

