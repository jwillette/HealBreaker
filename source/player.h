#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#endif


#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"

#endif


#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#endif


#include <string>
#include <iostream>
#include <sstream>

using namespace std;



#include <vector>
#include "bullet.h"



class Player
{
public:

	bool active;

	bool ronaDir;

	Mix_Chunk *fire;

	vector<Bullet> bulletList;

	string playerPath, playerPathR;

	SDL_Surface *surface, *surfaceR;

	SDL_Texture *texture, *textureR;

	SDL_Rect posRect;

	SDL_Rect drawRect;

	int frameWidth, frameHeight, textureWidth, textureHeight;

	float frameCounter;

	float xDir;
	float yDir;

	float speed;

	float pos_X, pos_Y;

	SDL_Texture *back, *mid, *front;
	SDL_Rect backR, midR, frontR;

	float playerHealth, maxHealth;

	float angle, oldAngle;



	Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	void OnControllerAxis(const SDL_ControllerAxisEvent event, float deltaTime);

	void OnControllerButton(const SDL_ControllerButtonEvent event);

	void Update(float deltaTime, SDL_Renderer *renderer);

	void Draw(SDL_Renderer *renderer);

	void GermHit();

	void BulletHit();

	void Walk(float deltaTime);

	~Player();

	void Reset();


private:

	void CreateBullet();
};
