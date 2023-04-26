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
    private:
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
        SDL_Renderer *Renderer;
        TTF_Font *Font;
        pair<int, int> selected[2];
        int score = 0;
        int startTime = 0;
        int timeLeft = 0;
        int countSelected = 0;
        int left = 0, right = 0, above = 0, below = 0;
        string oldHighScore;
        int newItem();
        bool drop = false;

    public:
        Game(SDL_Renderer *gRenderer, TTF_Font *gFont)
        {
            Renderer = gRenderer;
            Font = gFont;
        }

        void play(SDL_Event* e, int x, int y, bool* restart, bool& endG, int& pauseTime);

        bool loadImage();
        bool checkInit();
        void initGame();
        void render();

        void printScoreAndTime();
        void renderClock();
        void renderDrop(int timeDrop);
        void renderScore();
        void renderChoose();
        void renderEnd(bool &endG);
        void renderHint(int x, int y, int u, int v);
        void bombEffectRender(int x, int y);
        void stripedEffectRender(int x, int y, int status);

        void updateGame();
        void updateTouch(int mouseX, int mouseY);
        void swapItems(int x, int y, int u, int v);
        void updateBoard();
        void updateHighScore(int score);
        void outOfMove();
        bool checkClear();
        void decreaseRow();
        void hint();
        void getHighScore();

        int horizontal(int x, int y);
        int vertical(int x, int y);
        int eatCandy(int x, int y);
        void eatBomb(int x, int y);
        void eatStar(int type);
        void eatStriped(int col, int row);
};
#endif
