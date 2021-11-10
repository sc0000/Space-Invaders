#pragma once
#include "Pawn.h"

class Enemy :
    public Pawn
{
public:
    Enemy(SDL_Renderer* r, int srcX, int srcY, int winW, int winH, int w, int h, int vel, int& xOffset, int& yOffset);

    virtual void move() override;
    virtual void shoot() override;

    void setEnemies(std::vector<std::unique_ptr<Enemy>>& enem) 
    { 
        for (auto& e : enem)
            enemies.emplace_back(e.get()); 
    }

    void setShooting();

    static const int enemyPosOffset = Pawn::size * 2;

private:
    static const int defaultX = 4;
    static const int defaultY = 4;

    std::vector<Enemy*> enemies;

    Direction direction = Direction::Right;
    bool isShooting = true;
};

typedef std::unique_ptr<Enemy> EnemyPtr;
