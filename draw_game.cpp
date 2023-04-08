#include "draw_game.h"

SDL_Texture* DARWGAME::loadFromFile(string path, SDL_Renderer* gRenderer)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL)
	{
		std::cout<< "Unable to load image" << path.c_str() << "! SDL_image Error: "<< IMG_GetError()<< endl;
	}
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL)
		{
			std::cout<< "Unable to create texture from "<< path.c_str() << "! SDL Error:" << SDL_GetError() << endl;
		}
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool DARWGAME::loadMedia()
{
    bool success = true;
    if(!gCandy.loadFromFile("candy.png", gRenderer))
	{
        std::cout<< "Failed to load sprite sheet texture!\n";
        success = false;
    }
    else if(!start.loadFromFile("start.png", gRenderer))
    {
        std::cout<< "Failed to load start texture!\n";
        success = false;
    }
    else if(!choose.loadFromFile("choose.png", gRenderer)){
        std::cout<< "Failed to load choose texture!\n";
        success = false;
    }
    else if(!pause.loadFromFile("option.png", gRenderer)){
        std::cout<< "Failed to load option texture!\n";
        success = false;
    }
    else if(!backGround.loadFromFile("background.png", gRenderer)){
        std::cout<< "Failed to load background texture!\n";
        success = false;
    }
    else if(!gSelection.loadFromFile("endgame.png", gRenderer)){
        std::cout<< "Failed to load endgame texture!\n";
        success = false;
    }
    else{
        star.x = 0;
        star.y = 154;
        star.w = 75;
        star.h = 75;

        bomb.x = 77;
        bomb.y = 154;
        bomb.w = 75;
        bomb.h = 75;

        striped.x = 154;
        striped.y = 154;
        striped.w = 75;
        striped.h = 75;

        play[1].x = 0;
        play[1].y = 0;
        play[1].w = 270;
        play[1].h = 134;

        play[0].x = 0;
        play[0].y = 151;
        play[0].w = 270;
        play[0].h = 134;

        scoreboard.x = 0;
        scoreboard.y = 360;
        scoreboard.w = 413;
        scoreboard.h = 453;

        score.x = 440;
        score.y = 360;
        score.w = 100;
        score.h = 100;
    }
    return success;
}
