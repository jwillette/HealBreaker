#include "explode.h"

Explode::Explode(SDL_Renderer *renderer, string filePath, int enemy, float x, float y)
{
	active = false;
	
	string explodePath = filePath;

	if(enemy <= 1)
		explodePath = filePath + "germsplat.png";

	else
		explodePath = filePath + "virussplat.png";

	frameCounter = 0.0f;

	SDL_Surface *surface = IMG_Load(explodePath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	textureWidth = w;
	textureHeight = h;

	posRect.w = 180;
	posRect.h = 180;

	posRect.x = x;
	posRect.y = y;

	frameWidth = 180;
	frameHeight = 180;

	drawRect.x = 180;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;
}


void Explode::Reset()
{
	active = false;
}


void Explode::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
}


void Explode::Update(float deltaTime)
{
	if (active)
	{
		frameCounter += deltaTime;

		if (frameCounter >= .035f)
		{
			frameCounter = 0.0f;

			drawRect.x += frameWidth;

			if (drawRect.x >= textureWidth)
			{
				drawRect.x += 180;
				drawRect.y = 0;

			}

			else if (drawRect.x >= textureWidth && drawRect.y >= textureHeight)
			{
				drawRect.x = 0;
				drawRect.y = 0;

				active = false;

				posRect.x = -1000;
				posRect.y = -1000;
			}
		}
	}
}


Explode::~Explode()
{

}