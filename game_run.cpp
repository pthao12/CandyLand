#include "game_run.h"

bool init()
{
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
    {
        std::cout<< "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
            std::cout<< "Warning: Linear texture filtering not enabled! \n";
    }

    gWindow = SDL_CreateWindow("Candy Crush Saga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL)
    {
        std::cout<< "Window could not be created! SDL Error: "<< SDL_GetError() << endl;
        success = false;
    }
    else
    {
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(gRenderer == NULL)
        {
            std::cout<< "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
			{
				std::cout<< "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				success = false;
			}
            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                std::cout<< "SDL_mixer could not initialize! SDL_mixer Error: "<< Mix_GetError() << endl;
				success = false;
            }
            if (TTF_Init() == -1)
            {
				std::cout<< "SDL_ttf could not initialize! SDL_ttf Error: "<< TTF_GetError() << endl;
				success = false;
			}
        }
    }
    return success;
}
SDL_Texture* loadFromFile(string path, SDL_Renderer* gRenderer)
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

bool loadMedia()
{
    bool success = true;
    gFont = TTF_OpenFont("OrdinaryLove-VGwm0.ttf", 60);
    if(gFont == NULL)
    {
        cout<< "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << endl;
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
    else if(!gPlayButton.loadFromFile("playbutton.png", gRenderer)){
        std::cout<< "Failed to load playbutton texture!\n";
        success = false;
    }
    else{
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

        sizeofSettingButton.x = 0;
        sizeofSettingButton.y = 0;
        sizeofSettingButton.w = 81;
        sizeofSettingButton.h = 81;

        sizeofButton.x = 0;
        sizeofButton.y = 0;
        sizeofButton.w = 79;
        sizeofButton.h = 79;
    }
    return success;
}

void close()
{
    TTF_CloseFont(gFont);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
#include <SDL_events.h>
void run()
{
    if(!init())
	{
		cout<< "Failed to initialize!\n";
	}
    else{
        if(!loadMedia())
		{
			cout<< "Failed to load media!\n";
		}
        else
        {
            bool quit = false;
            SDL_Event e;
            int mouseX, mouseY;
            Uint32 frameStart;
            int frameTime;
            bool played = false;
            bool restart = true;
            bool pause = false;
            int timeLeft;
            int startTime;
            Game hehe(gRenderer, gFont);
            while(!quit)
            {
                frameStart = SDL_GetTicks();
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    if(!played){
                        //cout<< "moew"<< endl;
                        playButton.setPosition(466, 565);
                        HandlePlayButton(&e, playButton, played, pause, restart, &play[playButton.currentSprite]);
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(gRenderer);
                        start.render(0, 0, gRenderer);
                        gPlayButton.render(PLAY_BUTTON_POSX, PLAY_BUTTON_POSY, gRenderer, &play[playButton.currentSprite]);
                        SDL_RenderPresent(gRenderer);
                        //cout<< "hihi"<< endl;
                    }
                    if(played){
                        //cout<< "huhu"<< endl;
                        /*
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(gRenderer);
                        backGround.render(0,0, gRenderer);
                        */
                        settingButton.setPosition(866, 8);
                        HandleSettingButton(&e, settingButton, pause, &sizeofButton, gRenderer);
                        //cout<< x<<" "<<y<< endl;
                        //cout<< gRenderer<<" **** "<<endl;
                        if(pause == true)
                        {
                            LButton resume;
                            LButton home;
                            LButton restartGame;
                            LButton unmute;
                            LButton mute;

                            resume.setPosition(558, 216);
                            home.setPosition(558, 312);
                            restartGame.setPosition(558, 405);
                            mute.setPosition(558, 504);
                            unmute.setPosition(558, 504);
                            HandleResumeButton(&e, resume, pause, &sizeofButton, gRenderer);
                            HandleHomeButton(&e, home, played, restart, &sizeofButton, gRenderer);
                            HandleRestartButton(&e, restartGame, restart, pause, &sizeofButton, gRenderer);
                        }
                        if(pause == false && played == true)
                        {
                            int x, y;
                            SDL_GetMouseState( &x, &y );
                            hehe.play(&e, x, y, &restart);
                        }
                        //HandleSettingButton(&e, setting);
                    }
                }
            }
        }
    }
    close();
}
