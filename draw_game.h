#ifndef DRAW_GAME_H
#define DRAW_GAME_H

#include "LTexture.h"
#include "base.h"
#include "game_run.h"

namespace DARWGAME
{
    SDL_Texture* loadFromFile(string path, SDL_Renderer* gRenderer);
    bool loadMedia();
}
#endif
