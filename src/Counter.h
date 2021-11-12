#pragma once

#include "Pawn.h"
#include "MessageQueue.h"

class Counter :
    public GameObject
{
public:
    Counter(SDL_Renderer* r, SDL_Texture* t, int winW, int winH, int w, int h);
    
    virtual void render() override;

    void update() 
    { 
        hitPoints -= 1;
        if (hitPoints >= 0)
            counterRect.x -= 18;
    }
    
    int getCounter() { return hitPoints; }

private:
    SDL_Texture* texture;
    SDL_Rect dstRect2;
    SDL_Rect iconRect;
    SDL_Rect counterRect;

    int hitPoints = 5;
};

typedef std::unique_ptr<Counter> CounterPtr;