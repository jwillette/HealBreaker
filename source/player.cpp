#include "player.h"


const int JOYSTICK_DEAD_ZONE = 8000;


Player::Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;

	speed = 300.0f;

	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	playerAmmo = 10;

	xDir = 0;
	yDir = 0;

	angle = 0;
	oldAngle = 0;

	playerPath = filePath + "rona-walk-s.png";
	playerPathR = filePath + "rona-walk-sr.png";

	SDL_Surface *surface = IMG_Load(playerPath.c_str());
	SDL_Surface *surface2 = IMG_Load(playerPathR.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	textureR = SDL_CreateTextureFromSurface(renderer, surface2);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(surface2);

	posRect.x = x;
	posRect.y = y;

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	textureWidth = 2304;
	textureHeight = 449;

	//posRect.w = w;
	//posRect.h = h;

	pos_X = x;
	pos_Y = y;

	///
	int frame = 0;

	frameCounter = 0.0f;

	posRect.w = 288;
	posRect.h = 449;

	posRect.x = x;
	posRect.y = y;

	frameWidth = 288;
	frameHeight = 449;

	drawRect.x = 288;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;
	///

	ronaDir = 0;


	back = IMG_LoadTexture(renderer, (filePath + "health-bottom2.png").c_str());
	mid = IMG_LoadTexture(renderer, (filePath + "health-mid2.png").c_str());
	front = IMG_LoadTexture(renderer, (filePath + "health-top2.png").c_str());

	backR.x = midR.x = frontR.x = 10;
	backR.y = midR.y = frontR.y = 10;
	backR.w = midR.w = frontR.w = 270;
	backR.h = midR.h = frontR.h = 100;

	playerHealth = 100.0f;
	maxHealth = 100.0f;


	string bulletPath;

	bulletPath = filePath + "bullet.png";

	for (int i = 0; i < 10; i++)
	{
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);
		bulletList.push_back(tmpBullet);
	}
}


void Player::CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			Mix_PlayChannel(-1, fire, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
			bulletList[i].posRect.y = (posRect.y + (posRect.h / 2) - 33);

			bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w / 2);
			bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h / 2);

			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			if (xDir != 0)
			{
				bulletList[i].angle = angle;
			}
			else {
				bulletList[i].angle = oldAngle;
			}


			break;
		}
	}
}


void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if (event.which == 0)
	{
		if (event.button == 0 && playerAmmo > 0)
		{
			CreateBullet();
			playerAmmo--;
		}
	}
}


void Player::OnControllerAxis(const SDL_ControllerAxisEvent event, float deltaTime)
{
	if (event.which == 0)
	{
		if (event.axis == 0)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {

				xDir = 1.0f;
			}
			else {
				xDir = 0.0f;
			}
		}
	}
}


void Player::Reset()
{
	posRect.x = 100.0;
	posRect.y = 500.0;

	pos_X = posRect.x;
	pos_Y = posRect.y;

	xDir = 0;
	yDir = 0;

	active = true;

	playerHealth = 100;
	playerAmmo = 10;
}


void Player::Update(float deltaTime, SDL_Renderer *renderer)
{
	if (xDir != 0)
	{
		//		x = posRect.x - xDir;
		//		y = posRect.y - yDir;
		angle = atan2(yDir, xDir) * 180 / 3.14f;

		oldAngle = angle;

		//		xDirOld = xDir;
		//		yDirOld = yDir;

		// gives radians instead of degrees
		float radians = (angle * 3.14f) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		pos_X += (move_x)* deltaTime;
		pos_Y += (move_y)* deltaTime;

		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);



	}
	else {
		angle = oldAngle;
	}



	pos_X += (speed * xDir) * deltaTime;

	posRect.x = (int)(pos_X + 0.5f);


	if (posRect.x < 0) {
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if (posRect.x > 1024 - posRect.w) {
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if (posRect.y < 0) {
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if (posRect.y > 768 - posRect.h) {
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	if (xDir > 0)
	{
		for (int i = 0; i < 8; i++)
			Walk(deltaTime);
	}

	if (xDir < 0)
	{
		for (int i = 0; i < 8; i++)
			Walk(deltaTime);
	}


	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active)
		{
			bulletList[i].Update(deltaTime);
		}
	}
}


void Player::GermHit()
{
	playerHealth -= .015f;
	midR.w = playerHealth / maxHealth * 270;
}


void Player::BulletHit()
{
	playerHealth -= 5;
	midR.w = playerHealth / maxHealth * 270;
}


void Player::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, back, NULL, &backR);
	SDL_RenderCopy(renderer, mid, NULL, &midR);
	SDL_RenderCopy(renderer, front, NULL, &frontR);

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active)
		{
			bulletList[i].Draw(renderer);
		}
	}


	if (xDir > 0)
	{
		SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
		ronaDir = 0;
	}

	else if (xDir < 0)
	{
		SDL_RenderCopy(renderer, textureR, &drawRect, &posRect);
		ronaDir = 1;
	}

	else {

		if (ronaDir == 0) {
			SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
		}

		else {
			SDL_RenderCopy(renderer, textureR, &drawRect, &posRect);
		}

	}
}


void Player::Walk(float deltaTime)
{
	frameCounter += deltaTime;

	if (frameCounter >= 1)
	{
		frameCounter = 0.0f;

		drawRect.x += frameWidth;

		if (drawRect.x >= textureWidth)
		{
			drawRect.x += 288;

			if (drawRect.x >= textureWidth)
			{
				drawRect.x = 0;
			}
		}
	}
}


Player::~Player()
{
	//SDL_DestroyTexture(texture);
}
