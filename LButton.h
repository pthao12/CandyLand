#ifndef LBUTTON_H
#define LBUTTON_H

#include "base.h"
#include "LTexture.h"

enum LButtonSprite
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_TOTAL = 2
};

class LButton
{
	public:

        LButtonSprite currentSprite;

		LButton();

		void setPosition( int x, int y );

		bool handleEvent( SDL_Event* e, SDL_Rect* buttonSize);

	private:
		SDL_Point mPosition;
};

#endif
