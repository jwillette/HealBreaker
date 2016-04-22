#include "pickup.h"

Pickup::Pickup(SDL_Renderer *renderer, string filePath, int color, float x, float y)
{
	active = true;

	if (color == 0)
	{
		string basePath = filePath + "lilacCard.png";

		card = IMG_LoadTexture(renderer, basePath.c_str());

	}
	else if (color == 1) {

		string basePath = filePath + "yellowCard.png";

		card = IMG_LoadTexture(renderer, basePath.c_str());

	}
	else if (color == 2) {

		string basePath = filePath + "mintCard.png";

		card = IMG_LoadTexture(renderer, basePath.c_str());

	}
	else if (color == 3) {

		string basePath = filePath + "key1.png";

		card = IMG_LoadTexture(renderer, basePath.c_str());
	}

	else if (color == 4) {

		string basePath = filePath + "key2.png";

		card = IMG_LoadTexture(renderer, basePath.c_str());
	}

	else if (color == 5) {

		string basePath = filePath + "key3.png";

		card = IMG_LoadTexture(renderer, basePath.c_str());
	}

	cardRect.x = x;
	cardRect.y = y;

	int w, h;
	SDL_QueryTexture(card, NULL, NULL, &w, &h);
	cardRect.w = w;
	cardRect.h = h;

	posJ_X = cardRect.x;
	posJ_Y = cardRect.y;
}


void Pickup::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, card, NULL, &cardRect);
}


void Pickup::RonaMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;
	cardRect.x = (int)(posJ_X + 0.5f);
}


void Pickup::RonaMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	cardRect.y = (int)(posJ_Y + 0.5f);
}
