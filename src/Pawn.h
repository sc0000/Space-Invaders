#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "GameObject.h"
#include "Projectile.h"

class Pawn :
    public GameObject
{
public:
    Pawn(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel);
    
    virtual void render() override;
    void loadTexture(const char* file);

    virtual void shoot() {}
    void addProjectile(Direction dir);
    void moveProjectiles();

    void stop() { isRunning = false; }

    SDL_Rect srcRect;

    static const int size = 36;

protected:
    bool isRunning = true;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    std::vector<ProjectilePtr> projectiles;

    std::mutex mtx;
};

