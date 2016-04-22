#include <string>
#include <iostream>
#include <time.h>

using namespace std;
#include <vector>



#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#endif



#include "virusBullet.h"


class Virus
{
public:


	bool active;
	vector<VirusBullet> bulletList;
	string virusPath;

	SDL_Texture *vBase;
	SDL_Texture *vBarrel;

	SDL_Rect baseRect;
	SDL_Rect barrelRect;

	float x, y, virusAngle;

	SDL_Point center;

	float fireTime = 0.0f;
	float fireRate = 1000.0f;

	int health;

	Mix_Chunk *fire;
	Mix_Chunk *splat;


	Virus(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);
	void Update(float deltaTime, SDL_Rect playerRect);
	void Draw(SDL_Renderer *renderer);
	void Reset();
	void RemoveHealth();
	void CreateBullet(SDL_Rect target);
	void PlayerMoveX(float playerSpeed, float deltaTime);
	void PlayerMoveY(float playerSpeed, float deltaTime);
	float posB_X, posB_Y, posT_X, posT_Y;
};
