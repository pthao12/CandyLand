#ifndef game_run_h
#define game_run_h

#include "base.h"
#include "LTexture.h"
#include "game_utils.h"
#include "LButton.h"
#include "handle_button.h"

static LTexture gPlayButton;
static LTexture start;
static LButton playButton;
static LButton settingButton;
static SDL_Rect play[2];
static SDL_Rect sizeofSettingButton;
static SDL_Rect sizeofButton;


static LTexture pause;
static LTexture choose;

bool init();
bool loadMedia();
void close();
void run();
#endif
