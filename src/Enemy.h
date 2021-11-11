#pragma once
#include <iostream>

#include "Pawn.h"
#include "Player.h"

class Enemy :
    public Pawn
{
public:
    Enemy(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel, int& xOffset, int& yOffset, Player* p);
    ~Enemy()
    {}

    virtual void move() override;
    virtual void shoot() override;

    void startShootingThread()
    {
        t = std::thread(&Enemy::shoot, std::ref(*this));
    }

    /*void endShootingThread()
    {
        t.join();
    }*/

    void setEnemies(std::vector<std::unique_ptr<Enemy>>& enem) 
    {
        enemies.clear();
        for (auto& e : enem)
            enemies.emplace_back(e.get());
    }

    void setShooting();

    bool destroyed()
    {
        if (this != nullptr)
        {
            if (hitCounter > 4)
            {
                stop();
                enemies.clear();
                damages.clear();
                for (auto& p : projectiles)
                {
                    auto local = std::move(p);
                    local.reset();
                }

                t.join();
                return true;
            }

            return false;
        }
    }

    static const int enemyPosOffset = Pawn::size * 2;

private:
    static const int defaultX = 4;
    static const int defaultY = 4;

    std::vector<Enemy*> enemies;
    Player* player;
   
    Direction direction = Direction::Right;
    bool isShooting = true;
    std::thread t;
};

typedef std::unique_ptr<Enemy> EnemyPtr;
