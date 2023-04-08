#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "LButton.h"
#include "base.h"

#define fi first
#define se second
#define STAR 6
#define BOMB 7
#define STRIPED 8
#define CLOCK_X 313
#define CLOCK_Y 10
#define SPEED 5

void HandlePlayButton(SDL_Event* e, LButton& playButton, bool& play , bool& pause, bool& restart, SDL_Rect* buttonSize);

void HandleSettingButton(SDL_Event* e, LButton& settingButton, bool& pause, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleResumeButton(SDL_Event* e, LButton& resume, bool& pause, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleHomeButton(SDL_Event* e, LButton& home, bool& played, bool& restart, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleRestartButton(SDL_Event* e, LButton& restartGame, bool& restart, bool& pause, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleUnmuteButton(SDL_Event* e, LButton& unmute, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleMuteButton(SDL_Event* e, LButton& mute, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

class Game{
    public:
        int items[ROW_NUMBER][COLUMN_NUMBER];
        int clearCandy[ROW_NUMBER][COLUMN_NUMBER];
        int posX[ROW_NUMBER][COLUMN_NUMBER];
        int posY[ROW_NUMBER][COLUMN_NUMBER];
        int type[ROW_NUMBER][COLUMN_NUMBER];
        LTexture gCandy;
        LTexture backGround;
        LTexture gClock;
        LTexture endGame;
        LTexture choose;
        LTexture gTextTexture;
        SDL_Rect candy[ITEMS_NUMBER + 3];
        SDL_Rect clock[20];
        SDL_Rect board;
        SDL_Rect star;
        pair<int, int> selected[2];
        long long score = 0;
        int startTime = 0;
        int timeLeft = 0;
        int countSelected = 0;
        int left, right, above, below;
        SDL_Renderer *Renderer;
        TTF_Font *Font;
        Game(SDL_Renderer *gRenderer, TTF_Font *gFont)
        {
            Renderer = gRenderer;
            Font = gFont;
        }
        bool loadImage();
        void initGame();
        void printScoreAndTime();
        void render();
        void renderClock();
        void updateTouch(int mouseX, int mouseY);
        void swapItems(int x, int y, int u, int v);
        void updateGame();
        void play(SDL_Event* e, int x, int y, bool* restart);
        int horizontal(int x, int y);
        int vertical(int x, int y);
        int eatCandy(int x, int y);
        void decreaseRow();
        void renderDrop(int timeDrop);
        bool checkClear();
        void updateBoard();
        void eatBomb(int x, int y);
        void eatStar(int type);
        void eatStriped(int col, int row);
        void renderScore();
        void renderEnd();
    private:
        int newItem();
        bool checkInit();
};
#endif
