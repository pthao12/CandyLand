#ifndef base_h
#define base_h

#include <bits/stdc++.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

#define endl '\n'
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int PLAY_BUTTON_POSX = 466;
const int PLAY_BUTTON_POSY = 565;
const int COMMON_BUTTON_WIDTH = 77;
const int COMMON_BUTTON_HEIGHT = 77;
const int START_X = 150;
const int START_Y = 202;
const int ITEMS_SIZE = 75;
const int COLUMN_NUMBER = 12;
const int ROW_NUMBER = 7;
const int ITEMS_NUMBER = 6;
const int timeGame = 10;
static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static TTF_Font* gFont = NULL;
static Mix_Music* gMusic = NULL;

#endif
