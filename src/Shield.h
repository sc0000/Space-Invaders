#pragma once
#include "Pawn.h"

class Enemy;
class Player;

class Shield :
    public Pawn
{
public:
    Shield(SDL_Renderer* r, SDL_Texture* t, int srcX, int srcY, int winW, int winH, int w, int h, Player* p);

    void update();
    virtual bool destroyed() override;

private:
    Player* player;
};

typedef std::unique_ptr<Shield> ShieldPtr;