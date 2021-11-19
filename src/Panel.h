#pragma once

#include "GameObject.h"

class Panel :
    public GameObject
{
public:
    Panel(SDL_Renderer* r, SDL_Texture* t, int winW, int winH, int w, int h);

    virtual void render() override;
    void setTexture(SDL_Texture* t) { texture = t; }

private:
    SDL_Texture* texture;
};

typedef std::unique_ptr<Panel> PanelPtr;