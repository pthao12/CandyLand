#include "game_utils.h"

void HandlePlayButton(SDL_Event* e, LButton& playButton, bool& play, bool& pause, bool& restart, SDL_Rect* buttonSize)
{
    if(playButton.handleEvent(e, buttonSize))
    {
        switch(e -> type)
        {
        case SDL_MOUSEMOTION:
            playButton.currentSprite = BUTTON_MOUSE_OVER_MOTION;
            break;
        case SDL_MOUSEBUTTONDOWN:
            play = true;
            pause = false;
            restart = true;
            //Mix_PlayChannel(-1, gClick, 0);
            playButton.currentSprite = BUTTON_MOUSE_OVER_MOTION;
            break;
        }
    }
    else
    {
        playButton.currentSprite = BUTTON_MOUSE_OUT;
    }
}

void HandleSettingButton(SDL_Event* e, LButton& settingButton, bool& pause, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(settingButton.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            pause = true;
            LTexture setting;
            if(!setting.loadFromFile("setting.png", gRenderer))
            {
                cout<< "Failed to load setting texture!\n";
            }
            else
            {
                setting.render(0, 0, gRenderer);
                SDL_RenderPresent(gRenderer);
            }
        }
    }
}

void HandleResumeButton(SDL_Event* e, LButton& resume, bool& pause, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(resume.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            pause = false;
        }
    }
}

void HandleHomeButton(SDL_Event* e, LButton& home, bool& played, bool& restart, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(home.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            played = false;
        }
    }
}

void HandleRestartButton(SDL_Event* e, LButton& restartGame, bool& restart, bool& pause, SDL_Rect* buttonSize, SDL_Renderer *gRenderer)
{
    if(restartGame.handleEvent(e, buttonSize))
    {
        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            restart = true;
            pause = false;
        }
    }
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
    //cout<<endl<<"huhu"<<endl;
    bool success = true;
    //cout<< Renderer<<"___"<<endl;
    if(!backGround.loadFromFile("background.png", Renderer)){
        cout<< "Failed to load background texture!\n";
        success = false;
    }
    else if(!gCandy.loadFromFile("candy.png", Renderer))
	{
	    //cout<< Renderer<<"___"<<endl;
        cout<< "Failed to load candy texture!\n";
        success = false;
    }
    else if(!gClock.loadFromFile("clock.png", Renderer))
    {
        cout<< "Failed to load candy texture!\n";
    }
    else if(!endGame.loadFromFile("endgame.png", Renderer))
    {
        cout<< "Failed to load endgame texture!\n";
    }
    else if(!choose.loadFromFile("choose.png", Renderer))
    {
        cout<< "Failed to load choose texture!\n";
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                //cout<< i*3 + j<< endl;
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
                //cout<< i*5+j<<" "<< clock[i*5 + j].x<<" " <<clock[i*5 + j].y<<" "<< clock[i*5 + j].w<<" "<< clock[i*5 + j].h<<endl;
            }
        }

        board.x = 0;
        board.y = 0;
        board.w = 413;
        board.h = 438;

        star.x = 438;
        star.y = 0;
        star.h = 101;
        star.w = 101;
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
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            clearCandy[i][j] = 0;
        }
    }
    while(checkInit());
    //cắt ảnh các viên kẹo
    score = 0;
    countSelected = 0;
    timeLeft = timeGame;
    startTime = std::clock();
}
void Game::printScoreAndTime()
{
    cout<< "huhu"<<endl;
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
        gTextTexture.render(400, 20, Renderer);
    }
    else
    {
        std::cout << "Failed to render time texture" << endl;
        return;
    }
    text = "SCORE : " + to_string(score);
    if(gTextTexture.loadFromRenderedText(text, Font, textColor, Renderer))
    {
        gTextTexture.render(580, 20, Renderer);
    }
    else
        std::cout << "Failed to render score texture" << endl;
    cout<< "moew"<<endl;
}
void Game::render(){
    //initGame();

    //đẩy ảnh các viên kẹo lên
    SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(Renderer);
    backGround.render(0,0, Renderer);
    gClock.render(CLOCK_X, CLOCK_Y, Renderer, &clock[timeLeft% 20]);
    for(int i = 0; i < ROW_NUMBER; i++)
    {
        for(int j = 0; j < COLUMN_NUMBER; j++)
        {
            gCandy.render(posX[i][j], posY[i][j], Renderer, &candy[items[i][j]]);
        }
    }
    //cout<< score<<" "<<timeLeft<<endl;
    printScoreAndTime();
    SDL_RenderPresent(Renderer);
    cout<<"gou"<<endl;
}
void Game::updateTouch(int mouseX, int mouseY)
{
    //vị trí của chuột ở cột và hàng nào
    int col = (mouseX - START_X)/ITEMS_SIZE;
    int row = (mouseY - START_Y)/ITEMS_SIZE;
    //cout<< row<<" "<< col<<endl;
    if (row < 0 || col < 0 || row >= ROW_NUMBER || col >= COLUMN_NUMBER)
        return;
    if(countSelected == 1)
    {
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
    //cout<< x + 1<< " : "<< y + 1 <<" : "<<u + 1<<" : "<<v + 1<<endl;
    //cout<< items[y][x]<<" * "<< items[v][u]<<endl;
    //cout<< posX[y][x] <<" "<<posY[y][x] <<" "<< posX[v][u] <<" "<<posY[v][u]<<endl;
    for(int i = 0; i < 15; i++)
    {
        //cout<< posX[y][x] <<" "<<posY[y][x] <<" "<< posX[v][u] <<" "<<posY[v][u]<<endl;
        posX[y][x] += (u - x)*ITEMS_SIZE/15;
        posY[y][x] += (v - y)*ITEMS_SIZE/15;
        posX[v][u] += (x - u)*ITEMS_SIZE/15;
        posY[v][u] += (y - v)*ITEMS_SIZE/15;
        //cout<< (posX[y][x] - START_X)/75 + 1<<" "<<(posY[y][x]-START_Y)/75 + 1<<" "<<(posX[v][u] - START_X)/75 + 1<<" "<<(posY[v][u] - START_Y)/75 + 1<<endl;
        render();
    }
    //cout<< posX[y][x] <<" ^ "<<posY[y][x] <<" ^ "<< posX[v][u] <<" ^ "<<posY[v][u]<<endl;
    if(x != u)
        swap(posX[y][x], posX[v][u]);
    //cout<< posX[y][x] <<" "<<posY[y][x] <<" "<< posX[v][u] <<" "<<posY[v][u]<<endl;
    if(y != v)
        swap(posY[y][x], posY[v][u]);
    //cout<< posX[y][x] <<" "<<posY[y][x] <<" "<< posX[v][u] <<" "<<posY[v][u]<<endl;
    swap(items[y][x],items[v][u]);
    render();
    //cout<< (posX[y][x] - START_X)/75 + 1<<" - "<<(posY[y][x]-START_Y)/75 + 1<<" - "<<(posX[v][u] - START_X)/75 + 1<<" - "<<(posY[v][u] - START_Y)/75 + 1<<endl;
    //cout<< posX[y][x] <<" "<<posY[y][x] <<" "<< posX[v][u] <<" "<<posY[v][u]<<endl;
    //cout<< items[y][x]<<" * "<< items[v][u]<<endl;
}
void Game::renderDrop(int timeDrop)
{
    while(timeDrop)
    {
        //cout<< checkEnough()<< endl;
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
            //cout<< "r1 : " << r1<<" , r2 : "<<r2<<" , c1 : "<< c1<< endl;
            swap(items[r1][c1], items[r2][c1]);
            //posX[r1][c1] = posX[r2][c1];
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
                //posX[r1][c1] = START_X + c1*ITEMS_SIZE;
                if(r2 == -1 && items[r1][c1] == -1)
                    r2 = ITEMS_SIZE;
                else if(items[r1][c1] == -1)
                    r2 += ITEMS_SIZE;
                timeDrop = max(timeDrop, r2/SPEED);
                //cout<< "r1 : " << r1<<" , r2 : "<<r2<<" , c1 : "<< c1<< endl;
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
    //cout<< "huhu"<< endl;
    for(int i = ROW_NUMBER - 1; i >= 0; i--)
    {
        for(int j = COLUMN_NUMBER - 1; j >= 0; j--)
        {
            if(horizontal(j, i) + vertical(j, i) != 0)
            {
                items[i][j] = eatCandy(j, i);
                //cout<< i<<" "<< j<< " "<<items[i][j]<< endl;
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
    //cout<< x << " "<< y<<" "<<u<<" "<<v<<endl;
    //cout<< "countSelected : "<< countSelected<<endl;
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
        cout<<" x : "<< x<< " , y : "<< y<<" , u : "<< u<< " , v : "<< v<<endl;
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
                eatStriped(x, -1);
            else if(y == v)
                eatStriped(-1, y);
        }
        else if(items[v][u] == STRIPED)
        {
            if(x == u)
                eatStriped(x, -1);
            else if(y == v)
                eatStriped(-1, y);
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
    //cout<< items[y][x] << " "<< items[y][left - 1]<< endl;
    for(left; left > 0 && items[y][x] == items[y][left - 1] ; left--){}
        //cout<< "ai tem lep "<< items[y][left]<<endl;
    //cout<< items[y][x] << " "<< items[y][right + 1]<< endl;
    for(right; right < COLUMN_NUMBER - 1 && items[y][x] == items[y][right + 1]; right++){}
        //cout<< "ai tem rai "<<  items[y][right]<<endl;
    //cout<< "left : "<< left<<" , right : "<< right<<endl;
    if(right - left + 1 >= 3)
        return right - left + 1;
    else
        return 0;
}

int Game::vertical(int x, int y)
{
    above = y;
    below = y;
    //cout<< items[y][x] << " "<< items[above - 1][x]<< endl;
    if(items[y][x] == -1 || items[y][x] >= 6 ) return 0;
    for(above; above > 0 && items[y][x] == items[above - 1][x] ; above--){}
        //cout<< "ai tem o bov " << items[above][x]<<endl;
    //cout<< items[y][x] << " "<< items[above - 1][x]<< endl;
    for(below; below < ROW_NUMBER - 1 && items[y][x] == items[below + 1][x]; below++){}
        //cout<< "ai tem bi lou "<< items[below][x]<<endl;
    //cout<< "above : "<< above<<" , below : "<< below<<endl;
    if(below - above + 1 >= 3)
        return below - above + 1;
    else
        return 0;
}

int Game::eatCandy(int x, int y)
{

    int eatH = horizontal(x, y);
    int eatV = vertical(x, y);
    //cout<< x<<" __ "<<y<< endl;
    //cout<< "horizontal : "<< eatH<<" , vertical : "<< eatV<<endl;
    if(eatH == 5)
    {
        for(int i = left; i <= right; i++)
        {
            if(i != x)
                items[y][i] = -1;
        }
        return STAR;
    }
    else if(eatV == 5)
    {
        for(int i = above; i <= below; i++)
        {
            if(i != y)
                items[i][x] = -1;
        }
        return STAR;
    }
    else if(eatH == 3 && eatV == 3)
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
    updateBoard();
}
void Game::eatStriped(int col, int row)
{
    //cout<< "col : "<< col<< " , row : "<< row<< endl;
    if(row == -1)
    {
        for(int i = 0; i < ROW_NUMBER; i++)
        {
            //cout<< i<< " "<<col<<endl;
            items[i][col] = -1;
        }
    }
    else if(col == -1)
    {
        for(int i = 0; i < COLUMN_NUMBER; i++)
            items[row][i] = -1;
    }
}
void Game::renderScore()
{

}
void Game::play(SDL_Event* e, int x, int y, bool* restart){
    // chuột k có motion
    /*
    if( e->type == SDL_MOUSEMOTION ){

        SDL_GetMouseState( &x, &y );
        cout<< x << " : "<< y << '\n';
    }
    */

    //cout<< x << " : "<< y << '\n';
    if(*restart){
        if(loadImage())
            initGame();
        *restart = false;
    }
    //cout<< "restart : "<<*restart<<endl;
    if(*restart == false){
        timeLeft = timeGame - ((std::clock() - startTime) / (double)CLOCKS_PER_SEC);
        if(timeLeft == 0)
        {
            *restart = true;
            //renderEnd();
        }
        //cout<< timeLeft<<endl;
        if( e->type == SDL_MOUSEBUTTONDOWN )
        {
            //cout<<"hehe"<<endl;
            updateTouch(x, y);
        }
        updateGame();
        //cout<< "end____"<< endl;
    }
    render();
}

