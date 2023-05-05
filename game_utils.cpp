#include "game_utils.h"

Game::Game(SDL_Renderer *gRenderer, TTF_Font *gFont)
{
    Renderer = gRenderer;
    Font = gFont;
}
Game::~Game()
{
	score = 0;
    startTime = 0;
    timeLeft = 0;
    timeHint = 0;
    countSelected = 0;
    left = 0;
    right = 0;
    above = 0;
    below = 0;
    drop = false;
    sound = false;
}

void Game::play(SDL_Event* e, int x, int y, bool& restart, bool& endG, int &pauseTime){
    if(restart){
        if(loadImage())
            initGame();
        restart = false;
        pauseTime = 0;
        updateHighScore();
    }
    if(restart == false){
        timeLeft = timeGame - ((std::clock() - startTime - pauseTime) / (double)CLOCKS_PER_SEC);
        if(timeLeft <= 0)
        {
            restart = true;
            renderEnd(endG);
            return;
        }
        if( e->type == SDL_MOUSEBUTTONDOWN )
        {
            updateTouch(x, y);
            timeHint = timeLeft;
            sound = true;
        }
        updateGame();
    }
    render();
}

bool Game::loadImage()
{
    bool success = true;
    if(!backGround.loadFromFile("imgs/background.png", Renderer)){
        cout<< "Failed to load background texture!\n";
        success = false;
    }
    else if(!gCandy.loadFromFile("imgs/candy.png", Renderer))
	{
        cout<< "Failed to load candy texture!\n";
        success = false;
    }
    else if(!gClock.loadFromFile("imgs/clock.png", Renderer))
    {
        cout<< "Failed to load candy texture!\n";
    }
    else if(!choose.loadFromFile("imgs/choose.png", Renderer))
    {
        cout<< "Failed to load choose texture!\n";
    }
    else if(!gAnimation.loadFromFile("imgs/animation.png", Renderer))
    {
        cout<< "Failed to load animation texture!\n";
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                candy[i*3 + j].x = j*75 + j*2;
                candy[i*3 + j].y = i*75 + i*2;
                candy[i*3 + j].w = 75;
                candy[i*3 + j].h = 75;
            }
        }
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                clock[i*5 + j].x = 29 + 75*(j) + (j)*49;
                clock[i*5 + j].y = 74 + 75*(i) + (i)*11;
                clock[i*5 + j].w = 75;
                clock[i*5 + j].h = 75;
            }
        }
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                bombAnimation[i*4 + j].x = j*225;
                bombAnimation[i*4 + j].y = i*225;
                bombAnimation[i*4 + j].w = 225;
                bombAnimation[i*4 + j].h = 225;
            }
        }

        stripedEffect[HORIZONTAL].x = 1;
        stripedEffect[HORIZONTAL].y = 474;
        stripedEffect[HORIZONTAL].w = 75;
        stripedEffect[HORIZONTAL].h = 75;

        stripedEffect[VERTICAL].x = 130;
        stripedEffect[VERTICAL].y = 474;
        stripedEffect[VERTICAL].w = 75;
        stripedEffect[VERTICAL].h = 75;
     }
    return success;
}
int Game::newItem()
{
    return rand() % ITEMS_NUMBER;
}
bool Game::checkInit()
{
    bool check = false;
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            while(j + 2 < COLUMN_NUMBER && items[i][j] == items[i][j + 1] && items[i][j] == items[i][j + 2])
            {
                items[i][j + 1] = newItem();
                items[i][j + 2] = newItem();
                check = true;
            }
            while(i + 2 < ROW_NUMBER && items[i][j] == items[i + 1][j] && items[i][j] == items[i + 2][j])
            {
                items[i + 1][j] = newItem();
                items[i + 2][j] = newItem();
                check = true;
            }
        }
    }
    return check;
}

void Game::initGame()
{
    srand(time(0));
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            items[i][j] = newItem();
            posX[i][j] = START_X + ITEMS_SIZE*j;
            posY[i][j] = START_Y + ITEMS_SIZE*i;
        }
    }
    while(checkInit());
    getHighScore();
    score = 0;
    countSelected = 0;
    timeLeft = timeGame;
    startTime = std::clock();
}

void Game::renderCandy()
{
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            if(posY[i][j] >= START_Y)
               gCandy.render(posX[i][j], posY[i][j], Renderer, &candy[items[i][j]]);
        }
    }
}

void Game::renderClock()
{
    gClock.render(CLOCK_X, CLOCK_Y, Renderer, &clock[timeLeft% 20]);
}

void Game::renderDrop(int timeDrop)
{
    drop = true;
    while(timeDrop)
    {
        for(int i = 0; i < ROW_NUMBER; i++)
            for(int j = 0; j < COLUMN_NUMBER; j++)
                if(posY[i][j] != (START_Y + i*ITEMS_SIZE))
                {
                    posY[i][j] += SPEED;
                }
        render();
        timeDrop --;
    }
    loadChunk("sound/drop.wav");
    drop = false;
}

void Game::renderChoose()
{
    int x, y;
    SDL_GetMouseState( &x, &y );
    x = ceil((x - START_X)/ITEMS_SIZE)*ITEMS_SIZE + START_X;
    y = ceil((y - START_Y)/ITEMS_SIZE)*ITEMS_SIZE + START_Y;
    if(x >= START_X && x <= START_X + COLUMN_NUMBER*ITEMS_SIZE &&
       y >= START_Y && y <= START_Y + ROW_NUMBER*ITEMS_SIZE)
       choose.render(x, y, Renderer);
}

void Game::renderEnd(bool &endG)
{
    LTexture starScore;
    LTexture endGame;
    SDL_Rect star[3];
    if(!endGame.loadFromFile("imgs/endgame.png", Renderer))
    {
        cout<< "Failed to load endGame texture!\n";
        return;
    }
    else
    {
        endGame.render(0, 0, Renderer);
    }
    if(!starScore.loadFromFile("imgs/starScore.png", Renderer))
    {
        cout<< "Failed to load starScore texture!\n";
        return;
    }
    else
    {
        star[0] = {0, 0, 45, 45};
        star[1] = {102, 0, 74, 74};
        star[2] = {270, 0, 110, 110};
    }
    int numberStar = min(score/150, 3);
    if( numberStar == 0)
    {
        SDL_RenderPresent(Renderer);
    }
    if( numberStar >= 1 )
    {
        int desX[3];
        int desY[3];

        desX[0] = 462;
        desX[1] = 448;
        desX[2] = 430;

        desY[0] = 314;
        desY[1] = 300;
        desY[2] = 282;

        for(int i = 0; i < 3; i++)
        {
            starScore.render(desX[i], desY[i], Renderer, &star[i]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(250);
        }
    }
    if( numberStar >= 2 )
    {
        int desX[3];
        int desY[3];

        desX[0] = 582;
        desX[1] = 568;
        desX[2] = 550;

        desY[0] = 314;
        desY[1] = 300;
        desY[2] = 282;

        for(int i = 0; i <= 3; i++)
        {
            starScore.render(desX[i], desY[i], Renderer, &star[i]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(250);
        }
    }
    if( numberStar >= 3 )
    {
        int desX[3];
        int desY[3];

        desX[0] = 702;
        desX[1] = 688;
        desX[2] = 670;

        desY[0] = 314;
        desY[1] = 300;
        desY[2] = 282;

        for(int i = 0; i < 3; i++)
        {
            starScore.render(desX[i], desY[i], Renderer, &star[i]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(250);
        }
    }

    string text;
    text = "SCORE : " + to_string(score);
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};

    if(gTextTexture.loadFromRenderedText(text, Font, textColor, Renderer))
    {
        gTextTexture.render(450, 430, Renderer);
        SDL_RenderPresent(Renderer);
    }
    else
        cout << "Failed to render score texture" << endl;

    endG = true;
    return;
}

void Game::renderHint(int x, int y, int u, int v)
{
    if(timeHint - timeLeft < 5)
        return;
    else if(timeHint - timeLeft == 5 && sound)
    {
        sound = false;
        loadChunk("sound/hint.wav");
    }
    SDL_Rect hehe = {250, 473 , 75, 75};
    gAnimation.render(posX[y][x], posY[y][x], Renderer, &hehe);
    if(x != u || y != v)
        gAnimation.render(posX[v][u], posY[v][u], Renderer, &hehe);
}

int stringToInt(string s)
{
    int num = 0;
    for(unsigned int i = 0; i < s.length(); i++)
    {
        num = num*10 + int(s[i] - '0');
    }
    return num;
}

void Game::printScoreAndTime()
{
    string text;
    int minutes = timeLeft/60;
    int seconds = timeLeft - minutes*60;
    text = to_string(minutes);
    if(seconds < 10)
        text += " : 0" + to_string(seconds);
    else
        text += " : " + to_string(seconds);
    SDL_Color textColor = {0, 0, 0};
    if(gTextTexture.loadFromRenderedText(text, Font, textColor, Renderer))
    {
        gTextTexture.render(320, 20, Renderer);
    }
    else
    {
        cout << "Failed to render time texture" << endl;
        return;
    }

    text = to_string(score);

    if(gTextTexture.loadFromRenderedText(text, Font, textColor, Renderer))
    {
        gTextTexture.render(550, 20, Renderer);
    }
    else
        cout << "Failed to render score texture" << endl;

    if(gTextTexture.loadFromRenderedText(to_string(max(score, stringToInt(oldHighScore))), Font, textColor, Renderer))
    {
        gTextTexture.render(765, 20, Renderer);
    }
    else
        cout << "Failed to render score texture" << endl;
}

void Game::bombEffectRender(int x, int y)
{
    loadChunk("sound/bomb_sound.wav");
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 4; j++)
        {
            render();
            gAnimation.render(posX[y][x] - ITEMS_SIZE, posY[y][x] - ITEMS_SIZE, Renderer, &bombAnimation[i*4 + j]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(75);
        }
    render();
}

void Game::stripedEffectRender(int x, int y, int status)
{
    loadChunk("sound/striped_created.wav");
    if(status == HORIZONTAL)
    {
        int k = 0;
        while(1)
        {
            if(x + k < COLUMN_NUMBER)
                gAnimation.render(START_X + (x + k)*75, START_Y + y*75, Renderer, &stripedEffect[status]);
            if(x - k >= 0)
                gAnimation.render(START_X + (x - k)*75, START_Y + y*75, Renderer, &stripedEffect[status]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(75);
            k++;
            if(x - k < 0 && x + k > COLUMN_NUMBER) break;
        }
    }
    if(status == VERTICAL)
    {
        int k = 0;
        while(1)
        {
            if(y + k < ROW_NUMBER)
                gAnimation.render( START_X + x*75,  START_Y + (y + k)*75, Renderer, &stripedEffect[status]);
            if(y - k >= 0)
                gAnimation.render( START_X + x*75,  START_Y + (y - k)*75, Renderer, &stripedEffect[status]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(75);
            k++;
            if(y - k < 0 && y + k > ROW_NUMBER) break;
        }
    }
    render();
    SDL_Delay(100);
}

void Game::render()
{
    SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(Renderer);
    backGround.render(0,0, Renderer);
    renderClock();
    renderCandy();
    renderChoose();
    printScoreAndTime();
    if(!drop)
        hint();
    SDL_RenderPresent(Renderer);
}

int Game::horizontal(int x, int y)
{
    left = x;
    right = x;
    if(items[y][x] == -1 || items[y][x] >= 6 )
        return 0;
    for(left; left > 0 && items[y][x] == items[y][left - 1] ; left--);
    for(right; right < COLUMN_NUMBER - 1 && items[y][x] == items[y][right + 1]; right++);
    if(right - left + 1 >= 3)
        return right - left + 1;
    else
        return 0;
}

int Game::vertical(int x, int y)
{
    above = y;
    below = y;
    if(items[y][x] == -1 || items[y][x] >= 6 )
        return 0;
    for(above; above > 0 && items[y][x] == items[above - 1][x] ; above--);
    for(below; below < ROW_NUMBER - 1 && items[y][x] == items[below + 1][x]; below++);
    if(below - above + 1 >= 3)
        return below - above + 1;
    else
        return 0;
}

int Game::eatCandy(int x, int y)
{

    int eatH = horizontal(x, y);
    int eatV = vertical(x, y);
    if(eatH == 5)
    {
        for(int i = left; i <= right; i++)
        {
            if(i != x)
                items[y][i] = -1;
        }
        return STAR;
    }
    else if(eatV >= 5)
    {
        for(int i = above; i <= below; i++)
        {
            if(i != y)
                items[i][x] = -1;
        }
        return STAR;
    }
    else if(eatH >= 3 && eatV >= 3)
    {
        for(int i = left; i <= right; i++)
        {
            if(i != x)
                items[y][i] = -1;
        }
        for(int i = above; i <= below; i++)
        {
            if(i != y)
                items[i][x] = -1;
        }
        return BOMB;
    }
    else if(eatH == 4)
    {
        for(int i = left; i <= right; i++)
        {
            if(i != x)
                items[y][i] = -1;
        }
        return STRIPED;
    }
    else if(eatV == 4)
    {
        for(int i = above; i <= below; i++)
        {
            if(i != y)
                items[i][x] = -1;
        }
        return STRIPED;
    }
    else if(eatH == 3)
    {
        for(int i = left; i <= right; i++)
        {
            items[y][i] = -1;
        }
        return -1;
    }
    else if(eatV == 3)
    {
        for(int i = above; i <= below; i++)
        {
            items[i][x] = -1;
        }
        return -1;
    }
    return items[y][x];
}
void Game::eatStar(int type)
{
    loadChunk("sound/star_created.wav");
    if(type != STAR)
    {
        if(type > STAR)
            type = newItem();
        for(int i = 0; i < ROW_NUMBER; i++)
        {
            for(int j = 0; j < COLUMN_NUMBER; j++)
            {
                if(items[i][j] == type)
                    items[i][j] = -1;
            }
        }
    }
    else if(type == STAR)
    {
        for(int i = 0; i < ROW_NUMBER; i++)
            for(int j = 0; j < COLUMN_NUMBER; j++)
                items[i][j] = -1;
    }
}
void Game::eatBomb(int x, int y)
{
    for(int i = max(y - 1, 0); i < min(max(y - 1, 0) + 3, ROW_NUMBER); i++)
    {
        for(int j = max(x - 1, 0); j < min(max(x - 1, 0) + 3, COLUMN_NUMBER); j++)
        {
            if(items[i][j] < STAR)
                items[i][j] = -1;
        }
    }
    bombEffectRender(x, y);
    for(int i = max(y - 1, 0); i < min(max(y - 1, 0) + 3, ROW_NUMBER); i++)
    {
        for(int j = max(x - 1, 0); j < min(max(x - 1, 0) + 3, COLUMN_NUMBER); j++)
        {
            if(items[i][j] == STAR)
            {
                items[i][j] = -1;
                eatStar(newItem());
            }
            else if(items[i][j] == BOMB)
            {
                items[i][j] = -1;
                eatBomb(j, i);
            }
            else if(items[i][j] == STRIPED)
            {
                items[i][j] = -1;
                eatStriped(j, i, HORIZONTAL);
            }
        }
    }
    updateBoard();
}
void Game::eatStriped(int x, int y, int status)
{
    if(status == VERTICAL)
    {
        for(int i = 0; i < ROW_NUMBER; i++)
        {
            if(items[i][x] < STAR)
            {
                items[i][x] = -1;
            }
        }
        stripedEffectRender(x, y, VERTICAL);
        for(int i = 0; i < ROW_NUMBER; i++)
        {
            if(items[i][x] == STAR)
            {
                items[i][x] = -1;
                eatStar(newItem());
            }
            else if(items[i][x] == BOMB)
            {
                items[i][x] = -1;
                eatBomb(x, i);
            }
            else if(items[i][x] == STRIPED)
            {
                items[i][x] = -1;
                eatStriped(x, i, HORIZONTAL);
            }
        }
    }
    else if(status == HORIZONTAL)
    {
        for(int i = 0; i < COLUMN_NUMBER; i++)
        {
            if(items[y][i] < STAR)
            {
                items[y][i] = -1;
            }
        }
        stripedEffectRender(x, y, HORIZONTAL);
        for(int i = 0; i < COLUMN_NUMBER; i++)
        {
            if(items[y][i] == STAR)
            {
                items[y][i] = -1;
                eatStar(newItem());
            }
            else if(items[y][i] == BOMB)
            {
                items[y][i] = -1;
                eatBomb(i, y);
            }
            else if(items[y][i] == STRIPED)
            {
                items[y][i] = -1;
                eatStriped(i, y, VERTICAL);
            }
        }
    }
}

void Game::updateTouch(int mouseX, int mouseY)
{
    int col = (mouseX - START_X)/ITEMS_SIZE;
    int row = (mouseY - START_Y)/ITEMS_SIZE;
    if (row < 0 || col < 0 || row >= ROW_NUMBER || col >= COLUMN_NUMBER)
        return;
    if(countSelected == 1)
    {
        if((row == selected[0].y - 1 && col == selected[0].x) ||
               (row == selected[0].y+1 && col == selected[0].x) ||
               (row == selected[0].y && col == selected[0].x -1) ||
               (row == selected[0].y && col == selected[0].x +1))
            {
                selected[1].y = row;
                selected[1].x = col;
                countSelected = 2;
                return;
            }
        else
            countSelected = 0;
    }
    else
    {
        selected[0].y = row;
        selected[0].x = col;
        countSelected = 1;
    }
}
void Game::updateBoard()
{
    while(!checkClear());
}
void Game::updateGame()
{
    int x = selected[0].x,
        y = selected[0].y,
        u = selected[1].x,
        v = selected[1].y;
    if(countSelected == 2 && (abs(x-u) + abs(y-v)) != 1)
    {
        countSelected = 0;
        selected[0].x = 0;
        selected[0].y = 0;
        selected[1].x = 0;
        selected[1].y = 0;
        return;
    }
    if(items[y][x] == BOMB)
    {
        items[y][x] = -1;
        eatBomb(x, y);
        updateBoard();
        countSelected = 0;
        selected[0].x = 0;
        selected[0].y = 0;
        selected[1].x = 0;
        selected[1].y = 0;
        return;
    }
    else if(countSelected == 2)
    {
        if(items[v][u] == BOMB)
        {
            items[v][u] = -1;
            eatBomb(u, v);
            updateBoard();
            countSelected = 0;
            selected[0].x = 0;
            selected[0].y = 0;
            selected[1].x = 0;
            selected[1].y = 0;
            return;
        }
        swapItems(x, y, u, v);
        if(items[y][x] == STAR)
        {
            items[y][x] = -1;
            eatStar(items[v][u]);
        }
        else if(items[v][u] == STAR)
        {
            items[v][u] = -1;
            eatStar(items[y][x]);
        }
        else if(items[y][x] == STRIPED)
        {
            if(x == u)
            {
                items[y][x] = -1;
                eatStriped(x, y, VERTICAL);
            }
            else if(y == v)
            {
                items[y][x] = -1;
                eatStriped(x, y, HORIZONTAL);
            }
        }
        else if(items[v][u] == STRIPED)
        {
            if(x == u)
            {
                items[v][u] = -1;
                eatStriped(u, v, VERTICAL);
            }
            else if(y == v)
            {
                items[v][u] = -1;
                eatStriped(u, v, HORIZONTAL);
            }
        }
        else if(horizontal(x, y) + vertical(x, y) + horizontal(u, v) + vertical(u, v) == 0)
            swapItems(x, y, u, v);
        else
        {
            int xy = eatCandy(x, y);
            int uv = eatCandy(u, v);
            items[y][x] = xy;
            items[v][u] = uv;
            decreaseRow();
        }
        updateBoard();
        countSelected = 0;
        selected[0].x = 0;
        selected[0].y = 0;
        selected[1].x = 0;
        selected[1].y = 0;
    }
}
void Game::getHighScore()
{
    ifstream HighScoreFile ("HighScore.txt");
    if(HighScoreFile.is_open() && !HighScoreFile.eof())
    {
        HighScoreFile >> oldHighScore;
        HighScoreFile.close();
    }
}

void Game::updateHighScore()
{
    if(score > stringToInt(oldHighScore))
    {
        ofstream HighScoreFile ("HighScore.txt");
        HighScoreFile << to_string(score);
        HighScoreFile.close();
    }
}

void Game::swapItems(int x, int y, int u, int v)
{
    for(int i = 0; i < 15; i++)
    {
        posX[y][x] += (u - x)*ITEMS_SIZE/15;
        posY[y][x] += (v - y)*ITEMS_SIZE/15;
        posX[v][u] += (x - u)*ITEMS_SIZE/15;
        posY[v][u] += (y - v)*ITEMS_SIZE/15;
        render();
    }
    if(x != u)
        swap(posX[y][x], posX[v][u]);
    if(y != v)
        swap(posY[y][x], posY[v][u]);
    swap(items[y][x],items[v][u]);
}

void Game::decreaseRow()
{
    int c1, r1, r2;
    for(c1 = 0; c1 < COLUMN_NUMBER; c1++)
    {
        r1 = ROW_NUMBER - 1;
        while(r1 > 0)
        {
            while(r1 >= 0 && items[r1][c1] != -1)
                r1--;
            r2 = r1-1;
            while(r2 >= 0 && items[r2][c1] == -1)
                r2--;
            if(r2 < 0)
                break;
            swap(items[r1][c1], items[r2][c1]);
            posY[r1][c1] = posY[r2][c1];
        }
    }
    int timeDrop = 0;
    for(c1 = COLUMN_NUMBER - 1; c1 >= 0; c1--)
    {
        r2 = -1;
        for(r1 = ROW_NUMBER - 1; r1 >= 0; r1--)
        {
            if(items[r1][c1] == -1)
            {
                if(r2 == -1 && items[r1][c1] == -1)
                    r2 = ITEMS_SIZE;
                else if(items[r1][c1] == -1)
                    r2 += ITEMS_SIZE;
                timeDrop = max(timeDrop, r2/SPEED);
                posY[r1][c1] = START_Y - r2;
                items[r1][c1] = newItem();
            }
        }
        score += r2/ITEMS_SIZE;
    }
    if(timeDrop == 0)
        return;
    renderDrop(timeDrop);
}
bool Game::checkClear()
{
    for(int i = ROW_NUMBER - 1; i >= 0; i--)
    {
        for(int j = COLUMN_NUMBER - 1; j >= 0; j--)
        {
            if(horizontal(j, i) + vertical(j, i) != 0)
            {
                items[i][j] = eatCandy(j, i);
                if(items[i][j] >= 6 || items[i][j] == -1)
                {
                    decreaseRow();
                    return false;
                }
            }
            else if(items[i][j] == -1)
            {
                decreaseRow();
                return false;
            }
        }
    }
    return true;
}
void Game::hint()
{
    bool check = false;
    for(int i = ROW_NUMBER - 1; i >= 0; i--)
    {
        for(int j = COLUMN_NUMBER - 1; j >= 0; j--)
        {
            if(items[i][j] >= STAR)
            {
                check = true;
                renderHint(j, i, j, i);
                return;
            }
            if(i - 1 >= 0)
            {
                swap(items[i][j], items[i - 1][j]);
                if(vertical(j, i) >= 3 || vertical(j, i - 1) >= 3 ||
                   horizontal(j, i) >= 3 || horizontal(j, i - 1) >= 3)
                {
                    check = true;
                    swap(items[i][j], items[i - 1][j]);
                    renderHint(j, i, j, i - 1);
                    return;
                }
                else
                    swap(items[i][j], items[i - 1][j]);
            }
            if(i + 1 < ROW_NUMBER)
            {
                swap(items[i][j], items[i + 1][j]);
                if(vertical(j, i) >= 3 || vertical(j, i + 1) >= 3 ||
                   horizontal(j, i) >= 3 || horizontal(j, i + 1) >= 3)
                {
                    check = true;
                    swap(items[i][j], items[i + 1][j]);
                    renderHint(j, i, j, i + 1);
                    return;
                }
                else
                    swap(items[i][j], items[i + 1][j]);
            }
            if(j - 1 >= 0)
            {
                swap(items[i][j], items[i][j - 1]);
                if(vertical(j, i) >= 3 || vertical(j - 1, i) >= 3 ||
                   horizontal(j, i) >= 3 || horizontal(j - 1, i) >= 3)
                {
                    check = true;
                    swap(items[i][j], items[i][j - 1]);
                    renderHint(j, i, j - 1, i);
                    return;
                }
                else
                    swap(items[i][j], items[i][j - 1]);
            }
            if(j + 1 < COLUMN_NUMBER)
            {
                swap(items[i][j], items[i][j + 1]);
                if(vertical(j, i) >= 3 || vertical(j + 1, i) >= 3 ||
                   horizontal(j, i) >= 3 || horizontal(j + 1, i) >= 3)
                {
                    check = true;
                    swap(items[i][j], items[i][j + 1]);
                    renderHint(j, i, j + 1, i);
                    return;
                }
                else
                    swap(items[i][j], items[i][j + 1]);
            }
        }
    }

    if(check == false)
    {
        outOfMove();
    }
}

void Game::outOfMove()
{
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            items[i][j] = newItem();
            posX[i][j] = START_X + ITEMS_SIZE*j;
            posY[i][j] = START_Y + ITEMS_SIZE*i;
        }
    }
     while(checkInit());
}
