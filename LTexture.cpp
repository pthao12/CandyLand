#include "LTexture.h"

LTexture::LTexture()
{
    mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

void LTexture::free()
{
	if( mTexture != NULL )
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool LTexture::loadFromRenderedText(std::string textureText, TTF_Font* gFont, SDL_Color textColor, SDL_Renderer* gRenderer)
{
    free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if( textSurface == NULL )
	{
        std::cout<< "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
    }
    else{
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			std::cout<< "Unable to create texture from rendered text! SDL Error: "<< SDL_GetError() << endl;
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return mTexture != NULL;
}

bool LTexture::loadFromFile(string path, SDL_Renderer* gRenderer)
{
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL )
	{
		std::cout<< "Unable to load image" << path.c_str() << "! SDL_image Error: "<< IMG_GetError()<< endl;
	}
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == nullptr )
		{
			std::cout<< "Unable to create texture from "<< path.c_str() << "! SDL Error:" << SDL_GetError() << endl;
		}
        else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip)
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//gSelection.render(0, 0, gRenderer, &play[playButton.currentSprite]);
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
