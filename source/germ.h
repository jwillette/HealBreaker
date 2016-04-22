#include <string>
#include <iostream>

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

#include "explode.h"


class Germ
{
public:

	vector<Explode> explodeList;

	bool active;
	string germPath;
	SDL_Texture *germ;
	SDL_Rect germRect;
	float x, y, germAngle;
	Mix_Chunk *splat;
	float speed;
	int health;
	SDL_Point center;

	Germ(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);
	void Update(float deltaTime, SDL_Rect germRect);
	void Draw(SDL_Renderer *renderer);
	void Reset();
	void RemoveHealth();
	void MakeExplosion(int x, int y);
	void GermMoveX(float tankSpeed, float deltaTime);
	void GermMoveY(float tankSpeed, float deltaTime);
	float posT_X, posT_Y;
};
