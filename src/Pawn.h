#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "GameObject.h"
#include "Projectile.h"
#include "MessageQueue.h"

class Enemy;

class Pawn :
    public GameObject
{
public:
    Pawn(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel);
    ~Pawn()
    {}
    
    virtual void render() override;
    void loadTexture(const char* file);

    void setEnemies(std::vector<std::unique_ptr<Enemy>>& enem)
    {
        enemies.clear();
        for (auto& e : enem)
            enemies.emplace_back(e.get());
    }

    virtual void shoot() {}
    void addProjectile(Direction dir);
    void moveProjectiles();
    std::vector<std::unique_ptr<Projectile>>& getProjectiles() { return projectiles; }

    bool checkCollision(Projectile* projectile);

    void stop() { isRunning = false; }

    SDL_Rect srcRect;

    static const int size = 36;

protected:
    std::vector<Enemy*> enemies;
    std::vector<std::pair<int, int>> damages;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    bool isRunning = true;
    bool hasBeenHit = false;
    int hitCounter = 0;

    std::unique_ptr<MessageQueue<int>> hitCounterQueue;

    std::mutex mtx;
    std::mutex hitCounterMtx;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    SDL_Texture* damageTexture = nullptr;
};

