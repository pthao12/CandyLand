#include "LButton.h"

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	currentSprite = BUTTON_MOUSE_OUT;
}
void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}
bool LButton::handleEvent( SDL_Event* e, SDL_Rect* buttonSize)
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		if( x < mPosition.x )
		{
			inside = false;
		}
        else if( x > mPosition.x + buttonSize->w )
		{
			inside = false;
		}
		else if( y < mPosition.y )
		{
			inside = false;
		}
		else if( y > mPosition.y + buttonSize->h )
		{
			inside = false;
		}
        return inside;
	}
	return false;
}
void LButton::render(SDL_Rect* currentClip, SDL_Renderer* gRenderer, LTexture gButtonTexture)
{
    //gButtonTexture.render(0,0, gRenderer);
    //cout<< currentClip->y<< "hihihihhihihh"<< endl;
	gButtonTexture.render(0, 0, gRenderer, currentClip);
}
