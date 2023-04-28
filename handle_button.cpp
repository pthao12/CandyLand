#include "handle_button.h"

void loadChunk(string path)
{
    Mix_Chunk* gChunk = NULL;
    gChunk = Mix_LoadWAV( path.c_str() );
    if(gChunk == NULL)
    {
        cout<<"Failed to load "<< path.c_str() << "sound effect! SDL_mixer Error: " << Mix_GetError();
    }
    Mix_PlayChannel( -1, gChunk, 0 );
}
void HandlePlayButton(SDL_Event* e, LButton& playButton, bool& play, bool& pause, bool& restart, SDL_Rect* buttonSize)
{
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(playButton.handleEvent(e, buttonSize))
    {
        switch(e -> type)
        {
        case SDL_MOUSEMOTION:
            playButton.currentSprite = BUTTON_MOUSE_OVER_MOTION;
            break;
        case SDL_MOUSEBUTTONDOWN:

            loadChunk("sound/mouse_click.wav");

            play = true;
            pause = false;
            restart = true;
            playButton.currentSprite = BUTTON_MOUSE_OVER_MOTION;

            break;
        }
    }
    else
    {
        playButton.currentSprite = BUTTON_MOUSE_OUT;
    }
}

void HandleSettingButton(SDL_Event* e, LButton& settingButton, bool& pause, bool& music, bool& settingg, SDL_Rect* buttonSize, SDL_Renderer *gRenderer, int &pauseTime)
{
    if(settingButton.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");

            pauseTime = std::clock();
            pause = true;
            LTexture setting;
            if(settingg)
            {
                if(!setting.loadFromFile("imgs/blend.png", gRenderer))
                {
                    cout<< "Failed to load bled texture!\n";
                }
                else
                {
                    setting.render(0, 0, gRenderer);
                }
            }
            settingg = false;
            if(music)
            {
                if(!setting.loadFromFile("imgs/unmute.png", gRenderer))
                {
                    cout<< "Failed to load unmute texture!\n";
                }
                else
                {
                    setting.render(0, 0, gRenderer);
                    SDL_RenderPresent(gRenderer);
                }
            }
            else
            {
                if(!setting.loadFromFile("imgs/mute.png", gRenderer))
                {
                    cout<< "Failed to load mute texture!\n";
                }
                else
                {
                    setting.render(0, 0, gRenderer);
                    SDL_RenderPresent(gRenderer);
                }
                setting.free();
            }
        }
    }
}

void HandleResumeButton(SDL_Event* e, LButton& resume, bool& pause, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer, int& pauseTime)
{
    if(resume.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");
            pauseTime = std::clock() - pauseTime;
            pause = false;
            setting = true;
        }
    }
}

void HandleHomeButton(SDL_Event* e, LButton& home, bool& played, bool& restart, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(home.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");
            played = false;
            setting = true;
        }
    }
}

void HandleRestartButton(SDL_Event* e, LButton& restartGame, bool& restart, bool& pause, bool& endG, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(restartGame.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");
            restart = true;
            pause = false;
            endG = false;
            setting = true;
        }
    }
}

void HandleUnmuteButton(SDL_Event* e, LButton& unmute, bool& music, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(unmute.handleEvent(e, buttonSize))
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");
            music = true;
            LTexture setting;
            if(!setting.loadFromFile("imgs/unmute.png", gRenderer))
            {
                cout<< "Failed to load unmute texture!\n";
            }
            else
            {
                setting.render(0, 0, gRenderer);
                SDL_RenderPresent(gRenderer);
            }
        }
}

void HandleMuteButton(SDL_Event* e, LButton& mute, bool& music, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(mute.handleEvent(e, buttonSize))
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");
            music = false;
            LTexture setting;
            if(!setting.loadFromFile("imgs/mute.png", gRenderer))
            {
                cout<< "Failed to load mute texture!\n";
            }
            else
            {
                setting.render(0, 0, gRenderer);
                SDL_RenderPresent(gRenderer);
            }
        }

}

void HandleExitButton(SDL_Event* e, LButton& exit, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(exit.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            loadChunk("sound/mouse_click.wav");
            std::exit(0);
        }
    }
}
