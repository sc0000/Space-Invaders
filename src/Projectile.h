#pragma once

#include "GameObject.h"

class Projectile :
    public GameObject
{
public:
    Projectile(SDL_Rect ownerRect, Direction d, SDL_Renderer* r, int w, int h, int vel=12);

    virtual void render() override;
    virtual void move() override;
    
    Direction getDirection() { return direction; }
    
private:
    Direction direction;
    bool hasHit = false;
};

typedef std::unique_ptr<Projectile> ProjectilePtr;
