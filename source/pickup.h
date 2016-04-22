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



class Pickup
{
public:

	bool active;

	SDL_Texture *card;

	SDL_Rect cardRect;

	float posJ_X, posJ_Y;

	Pickup(SDL_Renderer *renderer, string filePath, int color, float x, float y);

	void Draw(SDL_Renderer *renderer);

	void Update(float deltaTime);

	void RonaMoveX(float tankSpeed, float deltaTime);

	void RonaMoveY(float tankSpeed, float deltaTime);
};
