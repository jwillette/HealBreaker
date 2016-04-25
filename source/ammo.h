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



class Ammo
{
public:

	bool active;

	SDL_Texture *ammo;
	SDL_Texture *ammoHUD[11];

	SDL_Rect ammoRect;

	SDL_Rect hudRect;

	float posJ_X, posJ_Y;

	Ammo(SDL_Renderer *renderer, string filePath, float x, float y);

	void DrawHUD(SDL_Renderer *renderer, int numberOfBullets);
	
	void DrawPU(SDL_Renderer *renderer);

	void Update(float deltaTime);

	void RonaMoveX(float tankSpeed, float deltaTime);

	void RonaMoveY(float tankSpeed, float deltaTime);
};
