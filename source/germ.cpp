#include "germ.h"

Germ::Germ(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	// activate the turret
	active = false;

	// fire sound
	splat = Mix_LoadWAV((audioPath + "splat.mp3").c_str());

	// create the turret base file path
	string basePath = filePath + "germ.png";

	// loat the image into the texture
	germ = IMG_LoadTexture(renderer, basePath.c_str());

	// set the SDL_Rect X and Y for the base image
	germRect.x = x;
	germRect.y = y;

	int w, h;
	SDL_QueryTexture(germ, NULL, NULL, &w, &h);
	germRect.w = w;
	germRect.h = h;

	posT_X = germRect.x;
	posT_Y = germRect.y;

	speed = 100;

	center.x = germRect.w / 2;
	center.y = germRect.h / 2;

	health = 10;

	// explosions for germs
	for (int i = 0; i < 6; i++)
	{
		Explode tmpExplode(renderer, filePath, 1, -1000, -1000);
		explodeList.push_back(tmpExplode);
	}
}

void Germ::Reset()
{
	// reset the x position off the screen
	germRect.x = -1000;

	// update the pos_X for precision
	posT_X = germRect.x;

	health = 10;

	active = false;
}

void Germ::MakeExplosion(int x, int y)
{
	for (int i = 0; i < explodeList.size(); i++)
	{
		if (explodeList[i].active == false)
		{
			explodeList[i].active = true;

			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			break;
		}
	}
}

void Germ::RemoveHealth()
{
	health -= 1;

	if (health <= 0)
	{
		Mix_PlayChannel(-1, splat, 0);
		MakeExplosion(posT_X,posT_Y);
		Reset();
	}
}

void Germ::GermMoveX(float tankSpeed, float deltaTime)
{
	posT_X += (tankSpeed)* deltaTime;

	germRect.x = (int)(posT_X + 0.5f);
}

void Germ::GermMoveY(float tankSpeed, float deltaTime)
{
	posT_Y += (tankSpeed)* deltaTime;

	germRect.y = (int)(posT_Y + 0.5f);
}

void Germ::Draw(SDL_Renderer *renderer)
{
	for (int i = 0; i < explodeList.size(); i++)
	{
		explodeList[i].Draw(renderer);
	}

	SDL_RenderCopyEx(renderer, germ, NULL, &germRect, germAngle, &center, SDL_FLIP_NONE);
}

void Germ::Update(float deltaTime, SDL_Rect tankRect)
{
	double distancex = (germRect.x - tankRect.x) * (germRect.x - tankRect.x);
	double distancey = (germRect.y - tankRect.y) * (germRect.y - tankRect.y);

	double calcdistance = sqrt(distancex + distancey);

	if (calcdistance <= 550)
	{
		active = true;
	}
	else {
		active = false;
	}

	if (active)
	{
		x = (tankRect.x + (tankRect.w / 2)) - (germRect.x + (germRect.w / 2));
		y = (tankRect.y + (tankRect.h / 2)) - (germRect.y + (germRect.h / 2));

		germAngle = atan2(y, x) * 180 / 3.14;

		float radians = (germAngle * 3.14) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		posT_X += (move_x)* deltaTime;
		posT_Y += (move_y)* deltaTime;

		germRect.x = (int)(posT_X + 0.5f);
		germRect.y = (int)(posT_Y + 0.5f);
	}

	for (int i = 0; i < explodeList.size(); i++)
	{
		explodeList[i].Update(deltaTime);
	}
}
