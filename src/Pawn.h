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
    Pawn(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, int winW, int winH, int w, int h, int vel=0);
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
    void setHitPoints(int hP) { hitPoints = hP; }
    virtual bool destroyed() { return false; }

    void stop() { isRunning = false; }

    static const int size = 36;

protected:
    std::vector<Enemy*> enemies;
    std::vector<std::pair<int, int>> damages;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    SDL_Rect srcRect;

    bool isRunning = true;
    bool hasBeenHit = false;
    int hitPoints = 0;

    std::mutex mtx;
    std::mutex hitCounterMtx;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    SDL_Texture* damageTexture = nullptr;
};

typedef std::unique_ptr<Pawn> PawnPtr;