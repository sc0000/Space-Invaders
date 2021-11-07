#pragma once

#include <memory>
#include <mutex>

#include "GameObject.h"

class Pawn :
    public GameObject
{
public:
    Pawn(const char* textureFile, int srcX, int srcY, SDL_Renderer* r, int winW, int winH, int w, int h, int vel);
    
    virtual void render() override;
    void loadTexture(const char* file);

    // void shoot();

    SDL_Rect srcRect;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;

    std::mutex mtx;
};

