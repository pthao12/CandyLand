#include "game_utils.h"

int stringToInt(string s)
{
    int num = 0;
    for(unsigned int i = 0; i < s.length(); i++)
    {
        num = num*10 + int(s[i] - '0');
    }
    return num;
}
int Game::newItem()
{
    return rand() % ITEMS_NUMBER;
}
bool Game::checkInit()
{
    bool check = false;
    //check xem có những hàng hay cột kẹo nào triệt tiêu k
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
bool Game::loadImage()
{
    bool success = true;
    if(!backGround.loadFromFile("imgs/background.png", Renderer)){
        std::cout<< "Failed to load background texture!\n";
        success = false;
    }
    else if(!gCandy.loadFromFile("imgs/candy.png", Renderer))
	{
        std::cout<< "Failed to load candy texture!\n";
        success = false;
    }
    else if(!gClock.loadFromFile("imgs/clock.png", Renderer))
    {
        std::cout<< "Failed to load candy texture!\n";
    }
    else if(!choose.loadFromFile("imgs/choose.png", Renderer))
    {
        std::cout<< "Failed to load choose texture!\n";
    }
    else if(!gAnimation.loadFromFile("imgs/animation.png", Renderer))
    {
        std::cout<< "Failed to load animation texture!\n";
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

        stripedEffect[HORIZONTAL].x = 0;
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
void Game::initGame()
{
    //khởi tạo hình cho các viên kẹo
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
    //cắt ảnh các viên kẹo
    getHighScore();
    score = 0;
    countSelected = 0;
    timeLeft = timeGame;
    startTime = std::clock();
}
void Game::getHighScore()
{
    fstream HighScoreFile ("HighScore.txt");
    if(HighScoreFile.is_open())
    {
        HighScoreFile >> oldHighScore;
        HighScoreFile.close();
    }
}
void Game::updateHighScore(int score)
{
    ofstream HighScoreFile ("HighScore.txt");
    if(score > stringToInt(oldHighScore))
    {
        HighScoreFile << to_string(score);
        HighScoreFile.close();
    }
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
        std::cout << "Failed to render time texture" << endl;
        return;
    }

    text = to_string(score);

    if(gTextTexture.loadFromRenderedText(text, Font, textColor, Renderer))
    {
        gTextTexture.render(550, 20, Renderer);
    }
    else
        std::cout << "Failed to render score texture" << endl;

    if(gTextTexture.loadFromRenderedText(to_string(max(score, stringToInt(oldHighScore))), Font, textColor, Renderer))
    {
        gTextTexture.render(765, 20, Renderer);
    }
    else
        std::cout << "Failed to render score texture" << endl;
}

void Game::bombEffectRender(int x, int y)
{
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 4; j++)
        {
            cout<< i*4 + j << endl;
            render();
            gAnimation.render(START_X + x*75, START_Y + y*75, Renderer, &bombAnimation[i*4 + j]);
            SDL_RenderPresent(Renderer);
            SDL_Delay(100);
        }
}

void Game::stripedEffectRender(int x, int y, int status)
{
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
            SDL_Delay(100);
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
            SDL_Delay(100);
            k++;
            if(y - k < 0 && y + k > ROW_NUMBER) break;
        }
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(Renderer);
    backGround.render(0,0, Renderer);
    gClock.render(CLOCK_X, CLOCK_Y, Renderer, &clock[timeLeft% 20]);
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            if(posY[i][j] >= START_Y)
               gCandy.render(posX[i][j], posY[i][j], Renderer, &candy[items[i][j]]);
        }
    }
    if(1)
    {
        int x, y;
        SDL_GetMouseState( &x, &y );
        x = ceil((x - START_X)/ITEMS_SIZE)*ITEMS_SIZE + START_X;
        y = ceil((y - START_Y)/ITEMS_SIZE)*ITEMS_SIZE + START_Y;
        if(x >= START_X && x <= START_X + COLUMN_NUMBER*ITEMS_SIZE &&
           y >= START_Y && y <= START_Y + ROW_NUMBER*ITEMS_SIZE)
           choose.render(x, y, Renderer);
    }
    printScoreAndTime();
    SDL_RenderPresent(Renderer);
}
void Game::updateTouch(int mouseX, int mouseY)
{
    //vị trí của chuột ở cột và hàng nào
    int col = (mouseX - START_X)/ITEMS_SIZE;
    int row = (mouseY - START_Y)/ITEMS_SIZE;
    if (row < 0 || col < 0 || row >= ROW_NUMBER || col >= COLUMN_NUMBER)
        return;
    if(countSelected == 1)
    {
        //SDL_RenderPresent(Renderer);
        if((row == selected[0].fi - 1 && col == selected[0].se) ||
               (row == selected[0].fi+1 && col == selected[0].se) ||
               (row == selected[0].fi && col == selected[0].se -1) ||
               (row == selected[0].fi && col == selected[0].se +1))
            {
                selected[1].fi = row;
                selected[1].se = col;
                countSelected = 2;
                return;
            }
        else
            countSelected = 0;
    }
    else
    {
        selected[0].fi = row;
        selected[0].se = col;
        countSelected = 1;
    }
}

void Game::swapItems(int x, int y, int u, int v)
{
    // đặt lại tên biến cho 5
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
    render();
}
void Game::renderDrop(int timeDrop)
{
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
void Game::updateBoard()
{
    while(!checkClear())
    {}
}
void Game::updateGame()
{
    int x = selected[0].se,
        y = selected[0].fi,
        u = selected[1].se,
        v = selected[1].fi;
    if(countSelected == 2 && (abs(x-u) + abs(y-v)) != 1)
    {
        countSelected = 0;
        selected[0].se = 0;
        selected[0].fi = 0;
        selected[1].se = 0;
        selected[1].fi = 0;
        return;
    }
    if(items[y][x] == BOMB)
    {
        eatBomb(x, y);
        updateBoard();
        countSelected = 0;
        selected[0].se = 0;
        selected[0].fi = 0;
        selected[1].se = 0;
        selected[1].fi = 0;
        return;
    }
    else if(countSelected == 2)
    {
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
                stripedEffectRender(x, y, VERTICAL);
                eatStriped(x, -1);
            }
            else if(y == v)
            {
                stripedEffectRender(x, y, HORIZONTAL);
                eatStriped(-1, y);
            }
        }
        else if(items[v][u] == STRIPED)
        {
            if(x == u)
            {
                stripedEffectRender(u, v, VERTICAL);
                eatStriped(u, -1);
            }
            else if(y == v)
            {
                stripedEffectRender(u, v, HORIZONTAL);
                eatStriped(-1, v);
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
        selected[0].se = 0;
        selected[0].fi = 0;
        selected[1].se = 0;
        selected[1].fi = 0;
    }
}
int Game::horizontal(int x, int y)
{
    left = x;
    right = x;
    if(items[y][x] == -1 || items[y][x] >= 6 ) return 0;
    for(left; left > 0 && items[y][x] == items[y][left - 1] ; left--){}
    for(right; right < COLUMN_NUMBER - 1 && items[y][x] == items[y][right + 1]; right++){}
    if(right - left + 1 >= 3)
        return right - left + 1;
    else
        return 0;
}

int Game::vertical(int x, int y)
{
    above = y;
    below = y;
    if(items[y][x] == -1 || items[y][x] >= 6 ) return 0;
    for(above; above > 0 && items[y][x] == items[above - 1][x] ; above--){}
    for(below; below < ROW_NUMBER - 1 && items[y][x] == items[below + 1][x]; below++){}
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
    if(type >= 0 && type <= 5)
    {
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
    updateBoard();
}
void Game::eatBomb(int x, int y)
{
    for(int i = max(x - 1, 0); i < min(max(x - 1, 0) + 3, COLUMN_NUMBER); i++)
    {
        for(int j = max(y - 1, 0); j < min(max(y - 1, 0) + 3, ROW_NUMBER); j++)
        {
            items[j][i] = -1;
        }
    }
    bombEffectRender(x - 1, y - 1);
    updateBoard();
}
void Game::eatStriped(int col, int row)
{
    if(row == -1)
    {
        for(int i = 0; i < ROW_NUMBER; i++)
            items[i][col] = -1;
    }
    else if(col == -1)
    {
        for(int i = 0; i < COLUMN_NUMBER; i++)
            items[row][i] = -1;
    }
}
void Game::renderEnd(bool &endG)
{
    LTexture starScore;
    LTexture endGame;
    SDL_Rect star[3];
    if(!endGame.loadFromFile("imgs/endgame.png", Renderer))
    {
        std::cout<< "Failed to load endGame texture!\n";
        return;
    }
    else
    {
        endGame.render(0, 0, Renderer);
    }
    if(!starScore.loadFromFile("imgs/starScore.png", Renderer))
    {
        std::cout<< "Failed to load starScore texture!\n";
        return;
    }
    else
    {
        star[0].x = 0;
        star[0].y = 0;
        star[0].w = 45;
        star[0].h = 45;

        star[1].x = 102;
        star[1].y = 0;
        star[1].w = 74;
        star[1].h = 74;

        star[2].x = 270;
        star[2].y = 0;
        star[2].w = 110;
        star[2].h = 110;
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
        std::cout << "Failed to render score texture" << endl;

    endG = true;
    updateHighScore(score);
    endGame.free();
    starScore.free();
    return;
}

void Game::play(SDL_Event* e, int x, int y, bool* restart, bool& endG){
    if(*restart){
        if(loadImage())
            initGame();
        *restart = false;
    }
    if(*restart == false){
        timeLeft = timeGame - ((std::clock() - startTime) / (double)CLOCKS_PER_SEC);
        if(timeLeft <= 0)
        {
            *restart = true;
            renderEnd(endG);
            return;
        }
        if( e->type == SDL_MOUSEBUTTONDOWN )
        {
            updateTouch(x, y);
        }
        updateGame();
    }
    render();
}

