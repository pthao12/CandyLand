#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "LButton.h"
#include "base.h"

#define fi first
#define se second
#define STAR 6
#define BOMB 7
#define STRIPED 8
#define CLOCK_X 240
#define CLOCK_Y 10
#define SPEED 5
#define HORIZONTAL 0
#define VERTICAL 1

class Game{
    public:
        int items[ROW_NUMBER][COLUMN_NUMBER];
        int posX[ROW_NUMBER][COLUMN_NUMBER];
        int posY[ROW_NUMBER][COLUMN_NUMBER];
        LTexture gCandy;
        LTexture backGround;
        LTexture gClock;
        LTexture choose;
        LTexture gTextTexture;
        LTexture gAnimation;
        SDL_Rect candy[ITEMS_NUMBER + 3];
        SDL_Rect clock[20];
        SDL_Rect bombAnimation[8];
        SDL_Rect stripedEffect[2];
        SDL_Rect board;
        pair<int, int> selected[2];
        int score = 0;
        int startTime = 0;
        int timeLeft = 0;
        int countSelected = 0;
        int left = 0, right = 0, above = 0, below = 0;
        string oldHighScore;
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
        void play(SDL_Event* e, int x, int y, bool* restart, bool& endG);
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
        void renderEnd(bool &endG);
        void bombEffectRender(int x, int y);
        void stripedEffectRender(int x, int y, int status);
        void getHighScore();
        void updateHighScore(int score);
    private:
        int newItem();
        bool checkInit();
};
#endif
