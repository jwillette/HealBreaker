#include "virus.h"

Virus::Virus(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;

	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());
	splat = Mix_LoadWAV((audioPath + "splat.mp3").c_str());

	string basePath = filePath + "vBase2.png";

	vBase = IMG_LoadTexture(renderer, basePath.c_str());

	string barrelPath = filePath + "vTurret2.png";

	vBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	// base
	baseRect.x = x;
	baseRect.y = y;

	int w, h;
	SDL_QueryTexture(vBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	// barrel
	barrelRect.x = x;
	barrelRect.y = y;

	SDL_QueryTexture(vBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;

	center.x = w/2;
	center.y = h/2;

	string bulletPath = filePath + "virusBullet.png";

	for (int i = 0; i < 10; i++)
	{
		VirusBullet tmpBullet(renderer, bulletPath, 1500, 1500);
		bulletList.push_back(tmpBullet);
	}

	srand(time(NULL));

	// update the float values for movement
	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;

	health = 10;


}


void Virus::Reset()
{
	// reset the x position off the screen
	barrelRect.x = -2000;
	baseRect.x = -2000;

	// update the pos_X for precision
	posT_X = barrelRect.x;
	posB_X = baseRect.x;

	health = 10;

	active = false;
}


void Virus::RemoveHealth()
{
	health -= 2;

	if (health <= 0)
	{
		Mix_PlayChannel(-1, splat, 0);
		Reset();
	}
}


void Virus::PlayerMoveX(float playerSpeed, float deltaTime)
{
	posB_X += (playerSpeed)* deltaTime;
	posT_X += (playerSpeed)* deltaTime;

	baseRect.x = (int)(posB_X + 0.5f);
	barrelRect.x = (int)(posT_X + 0.5f);
}


void Virus::PlayerMoveY(float playerSpeed, float deltaTime)
{
	posB_Y += (playerSpeed)* deltaTime;
	posT_Y += (playerSpeed)* deltaTime;

	baseRect.y = (int)(posB_Y + 0.5f);
	barrelRect.y = (int)(posT_Y + 0.5f);
}


void Virus::Draw(SDL_Renderer *renderer)
{

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active)
		{
			bulletList[i].Draw(renderer);
		}
	}

	SDL_RenderCopy(renderer, vBase, NULL, &baseRect);
	SDL_RenderCopyEx(renderer, vBarrel, NULL, &barrelRect, virusAngle, &center, SDL_FLIP_NONE);
}


void Virus::Update(float deltaTime, SDL_Rect playerRect)
{
	x = (playerRect.x + (playerRect.w / 2)) - (baseRect.x + (baseRect.w / 2));
	y = (playerRect.y + (playerRect.h / 2)) - (baseRect.y + (baseRect.h / 2));
	virusAngle = atan2(y, x) * 180 / 3.14;

	if (SDL_GetTicks() > fireTime)
	{
		if (baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768)
		{
			CreateBullet(playerRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 3 + 1) * 1000;
	}

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active)
		{
			bulletList[i].Update(deltaTime);
		}
	}
}


void Virus::CreateBullet(SDL_Rect target)
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			bulletList[i].Start(target, baseRect);

			Mix_PlayChannel(-1, fire, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = ((baseRect.x + (baseRect.w / 2)) - (bulletList[i].posRect.w / 2));
			bulletList[i].posRect.y = ((baseRect.y + (baseRect.w / 2)) - (bulletList[i].posRect.h / 2));

			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			break;
		}
	}
}
