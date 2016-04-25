#include "ammo.h"

Ammo::Ammo(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = true;

	string basePath = filePath + "ammo.png";
	ammo = IMG_LoadTexture(renderer, basePath.c_str());

	ammoRect.x = x;
	ammoRect.y = y;

	int w, h;
	SDL_QueryTexture(ammo, NULL, NULL, &w, &h);
	ammoRect.w = w;
	ammoRect.h = h;

	posJ_X = ammoRect.x;
	posJ_Y = ammoRect.y;



	string hudPath[11]; 
	hudPath[0] = filePath + "0.png";
	hudPath[1] = filePath + "1.png";
	hudPath[2] = filePath + "2.png";
	hudPath[3] = filePath + "3.png";
	hudPath[4] = filePath + "4.png";
	hudPath[5] = filePath + "5.png";
	hudPath[6] = filePath + "6.png";
	hudPath[7] = filePath + "7.png";
	hudPath[8] = filePath + "8.png";
	hudPath[9] = filePath + "9.png";
	hudPath[10] = filePath + "10.png";
	
	for (int i = 0; i < 11; i++)
		ammoHUD[i] = IMG_LoadTexture(renderer, hudPath[i].c_str());

	hudRect.x = 15;
	hudRect.y = 150;

	int w2, h2;
	SDL_QueryTexture(ammoHUD[10], NULL, NULL, &w2, &h2);
	hudRect.w = w2;
	hudRect.h = h2;
}


void Ammo::DrawHUD(SDL_Renderer *renderer, int numberOfBullets)
{
	SDL_RenderCopy(renderer, ammoHUD[numberOfBullets], NULL, &hudRect);
}


void Ammo::DrawPU(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, ammo, NULL, &ammoRect);
}


void Ammo::RonaMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;
	ammoRect.x = (int)(posJ_X + 0.5f);
}


void Ammo::RonaMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	ammoRect.y = (int)(posJ_Y + 0.5f);
}
