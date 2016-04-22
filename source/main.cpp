#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#endif

#if defined(_WIN32) || (_WIN64)

#include <direct.h>
#define getcwd _getcwd

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

#if defined(__linux__)

#include <unistd.h>

#endif



#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "pickup.h"
#include "germ.h"
#include "virus.h"

using namespace std;



int bkgdSpeed = 50;

SDL_Rect bkgd1Pos, bkgd2Pos;

float b1pos_X = 0, b1pos_Y = 0;
float b2pos_X = 0, b2pos_Y = -768;

float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;



void UpdateBackground()
{
	b1pos_Y += (bkgdSpeed * 1) * deltaTime;

	bkgd1Pos.y = (int)(b1pos_Y + 0.5f);

	if (bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y = -768;
		b1pos_Y = bkgd1Pos.y;
	}

	b2pos_Y += (bkgdSpeed * 1) * deltaTime;

	bkgd2Pos.y = (int)(b2pos_Y + 0.5f);

	if (bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y = -768;
		b2pos_Y = bkgd2Pos.y;
	}
}


const int JOYSTICK_DEAD_ZONE = 8000;

float xDir, yDir;

float pos_X, pos_Y;

SDL_Rect cursorPos, activePos;

int cursorSpeed = 400;


void moveCursor(const SDL_ControllerAxisEvent event)
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

		if (event.axis == 1)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			}
			else {
				yDir = 0.0f;
			}
		}
	}
}


void UpdateCursor(float deltaTime)
{
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if (cursorPos.x < 0)
	{
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if (cursorPos.x > 1024 - cursorPos.w)
	{
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	if (cursorPos.y < 0)
	{
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if (cursorPos.y > 768 - cursorPos.h)
	{
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}


bool startOver = false, backstoryOver = false, instructionsOver = false, quitOver = false,
menuOver = false, level1Over = false;







int main(int argc, char* argv[])
{
	srand(time(NULL));

#if defined(_WIN32) || (_WIN64)

	string cwd(getcwd(NULL, 0));

	string images_dir = cwd + "\\HealBreaker\\images\\";

	string audio_dir = cwd + "\\HealBreaker\\audio\\";

#endif


#if defined(__APPLE__)

	string cwd(getcwd(NULL, 0));

	string images_dir = cwd + "/HealBreaker/images/";

	string audio_dir = cwd + "/HealBreaker/audio/";

#endif


#if defined(__linux__)

	string cwd(getcwd(NULL, 0));

	string images_dir = cwd + "/HealBreaker/images/";

	string audio_dir = cwd + "/HealBreaker/audio/";

#endif



	SDL_Window *window;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("An SDL2 window", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}



	SDL_Renderer *renderer = NULL;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	/////////////////////////////////////////
	//     ***** Create Background *****
	/////////////////////////////////////////

	SDL_Surface *surface = IMG_Load((images_dir + "bg.png").c_str());

	SDL_Texture *bkgd1;
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Texture *bkgd2;
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);


	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 768;

	bkgd2Pos.x = 0;
	bkgd2Pos.y = -768;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 768;



	///////////////////////////////////////////
	//        ***** Create Menu *****
	///////////////////////////////////////////


	// ***** Title ***** //

	surface = IMG_Load((images_dir + "title2.png").c_str());
	SDL_Texture *title;
	title = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect titlePos;

	titlePos.x = 240;
	titlePos.y = 20;
	titlePos.w = 544;
	titlePos.h = 75;


	// ***** Rona Graphic ***** //

	surface = IMG_Load((images_dir + "rona-graphic2.png").c_str());
	SDL_Texture *ronaMenu;
	ronaMenu = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect ronaMenuPos;

	ronaMenuPos.x = 10;
	ronaMenuPos.y = 68;
	ronaMenuPos.w = 498;
	ronaMenuPos.h = 700;


	// ***** Start Game ***** //

	surface = IMG_Load((images_dir + "start-d2.png").c_str());
	SDL_Texture *startN;
	startN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "start-o.png").c_str());
	SDL_Texture *startO;
	startO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect startPos;

	startPos.x = 646;
	startPos.y = 255;
	startPos.w = 363;
	startPos.h = 52;


	// ***** Instructions ***** //

	surface = IMG_Load((images_dir + "instructions-d2.png").c_str());
	SDL_Texture *instructionsN;
	instructionsN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "instructions-o.png").c_str());
	SDL_Texture *instructionsO;
	instructionsO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect instructionsPos;

	instructionsPos.x = 636;
	instructionsPos.y = 384;
	instructionsPos.w = 382;
	instructionsPos.h = 54;


	// ***** Backstory ***** //

	surface = IMG_Load((images_dir + "backstory-d2.png").c_str());
	SDL_Texture *backstoryN;
	backstoryN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "backstory-o.png").c_str());
	SDL_Texture *backstoryO;
	backstoryO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect backstoryPos;

	backstoryPos.x = 676;
	backstoryPos.y = 512;
	backstoryPos.w = 304;
	backstoryPos.h = 67;


	// ***** Quit ***** //

	surface = IMG_Load((images_dir + "quit-d2.png").c_str());
	SDL_Texture *quitN;
	quitN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "quit-o.png").c_str());
	SDL_Texture *quitO;
	quitO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect quitPos;

	quitPos.x = 659;
	quitPos.y = 640;
	quitPos.w = 337;
	quitPos.h = 54;



	//////////////////////////////////////////////////
	//        ***** Create Instructions *****
	//////////////////////////////////////////////////


	// ***** Instructions ***** //

	surface = IMG_Load((images_dir + "instructions-text2.png").c_str());
	SDL_Texture *instructionsText;
	instructionsText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect instructionsTextPos;

	instructionsTextPos.x = 20;
	instructionsTextPos.y = 160;
	instructionsTextPos.w = 583;
	instructionsTextPos.h = 559;


	// ***** Backstory ***** //

	surface = IMG_Load((images_dir + "backstory-text.png").c_str());
	SDL_Texture *backstoryText;
	backstoryText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect backstoryTextPos;

	backstoryTextPos.x = 20;
	backstoryTextPos.y = 160;
	backstoryTextPos.w = 583;
	backstoryTextPos.h = 559;


	// ***** Main Menu ***** //

	surface = IMG_Load((images_dir + "menu.png").c_str());
	SDL_Texture *menuN;
	menuN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "menu-o.png").c_str());
	SDL_Texture *menuO;
	menuO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect menuNPos;

	menuNPos.x = 666;
	menuNPos.y = 512;
	menuNPos.w = 330;
	menuNPos.h = 54;


	// ***** Start Game ***** //

	SDL_Rect startPosInst;

	startPosInst.x = 646;
	startPosInst.y = 255;
	startPosInst.w = 363;
	startPosInst.h = 52;


	// ***** Backstory ***** //

	SDL_Rect backstoryPosInst;

	backstoryPosInst.x = 676;
	backstoryPosInst.y = 384;
	backstoryPosInst.w = 304;
	backstoryPosInst.h = 67;



	///////////////////////////////////////////
	//        ***** Create Win *****
	///////////////////////////////////////////


	// ***** Win ***** //

	surface = IMG_Load((images_dir + "win.png").c_str());
	SDL_Texture *winText;
	winText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect winTextPos;

	winTextPos.x = 137;
	winTextPos.y = 20;
	winTextPos.w = 720;
	winTextPos.h = 218;


	// ***** Win graphic ***** //

	surface = IMG_Load((images_dir + "wingraphic.png").c_str());
	SDL_Texture *winGraphic;
	winGraphic = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect winGraphicPos;

	winGraphicPos.x = 0;
	winGraphicPos.y = 0;
	winGraphicPos.w = 1024;
	winGraphicPos.h = 768;


	// ***** Play Again ***** //

	surface = IMG_Load((images_dir + "playagain-d.png").c_str());
	SDL_Texture *playN;
	playN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "playagain-o.png").c_str());
	SDL_Texture *playO;
	playO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect playNPos;

	playNPos.x = 680;
	playNPos.y = 700;
	playNPos.w = 326;
	playNPos.h = 70;


	// ***** Reposition Menu ***** //

	SDL_Rect menuWPos;

	menuWPos.x = 25;
	menuWPos.y = 700;
	menuWPos.w = 330;
	menuWPos.h = 54;



	///////////////////////////////////////////
	//        ***** Create Lose *****
	///////////////////////////////////////////


	// ***** Lose ***** //

	surface = IMG_Load((images_dir + "lose.png").c_str());
	SDL_Texture *loseText;
	loseText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect loseTextPos;

	loseTextPos.x = 110;
	loseTextPos.y = 30;
	loseTextPos.w = 800;
	loseTextPos.h = 226;


	// ***** Dim bg ***** //

	surface = IMG_Load((images_dir + "losefilter2.png").c_str());
	SDL_Texture *bgFilter;
	bgFilter = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect bgFilterPos;

	bgFilterPos.x = 0;
	bgFilterPos.y = 0;
	bgFilterPos.w = 1024;
	bgFilterPos.h = 768;



	/////////////////////////////////////////
	//        ***** Create Cursor *****
	/////////////////////////////////////////
	surface = IMG_Load((images_dir + "cursor.png").c_str());

	SDL_Texture *cursor;

	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	cursorPos.x = 0;
	cursorPos.y = 0;
	cursorPos.w = 64;
	cursorPos.h = 64;

	activePos.x = 5;
	activePos.y = 5;
	activePos.w = 15;
	activePos.h = 15;





	/////////////////////////////////////////
	//     ***** Create Controller *****
	/////////////////////////////////////////

	SDL_GameControllerEventState(SDL_ENABLE);

	SDL_GameController* gGameController0 = NULL;

	gGameController0 = SDL_GameControllerOpen(0);

	SDL_GameController* gGameController1 = NULL;
	gGameController1 = SDL_GameControllerOpen(1);



	SDL_Event event;

	enum GameState { MENU, INSTRUCTIONS, LEVEL1, LEVEL2, BACKSTORY, WIN, LOSE };

	GameState gameState = MENU;

	bool menu, instructions, level1, level2, backstory, win, lose, quit;
	quit = false;



	/////////////////////////////////////////
	//           ***** Audio *****
	/////////////////////////////////////////

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "Surreal-Chase.mp3").c_str());
	Mix_Music *bgl1 = Mix_LoadMUS((audio_dir + "inside-Insane-Gameplay.wav").c_str());
	Mix_Music *bgl2 = Mix_LoadMUS((audio_dir + "outside-Funky-Gameplay_v001.mp3").c_str());
	Mix_Music *bglose = Mix_LoadMUS((audio_dir + "lose-Escape.mp3").c_str());

	if (!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "select.wav").c_str());

	//Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "Germ.wav").c_str());

	bool alreadyOver = false;



	/////////////////////////////////////////
	//           ***** LEVEL 1 *****
	/////////////////////////////////////////

	// bg
	SDL_Texture *l1bkgd = IMG_LoadTexture(renderer, (images_dir + "l1-bg.png").c_str());
	SDL_Rect l1bkgdRect;
	l1bkgdRect.x = 0;
	l1bkgdRect.y = 0;
	l1bkgdRect.w = 4096;
	l1bkgdRect.h = 768;
	float X_pos = 0.0f, Y_pos = 0.0f;


	// level 1 door
	SDL_Texture *door = IMG_LoadTexture(renderer, (images_dir + "door.png").c_str());
	SDL_Rect doorRect;
	doorRect.x = 3670;
	doorRect.y = 145;
	doorRect.w = 378;
	doorRect.h = 490;
	float doorX_pos = 3670.0f, doorY_pos = 145.0f;


	// card HUD
	SDL_Texture *Cardbkgd = IMG_LoadTexture(renderer, (images_dir + "cards-empty.png").c_str());
	SDL_Rect CardbkgdRect;
	CardbkgdRect.x = 725;
	CardbkgdRect.y = 16;
	CardbkgdRect.w = 284;
	CardbkgdRect.h = 91;

	SDL_Texture *lilac = IMG_LoadTexture(renderer, (images_dir + "cards-lilac.png").c_str());

	SDL_Texture *yellow = IMG_LoadTexture(renderer, (images_dir + "cards-yellow.png").c_str());

	SDL_Texture *mint = IMG_LoadTexture(renderer, (images_dir + "cards-mint.png").c_str());

	bool haveLilac = false;
	bool haveYellow = false;
	bool haveMint = false;

	Pickup lilacCard = Pickup(renderer, images_dir.c_str(), 0, 750.0f, 400.0f);
	Pickup yellowCard = Pickup(renderer, images_dir.c_str(), 1, 1600.0f, 400.0f);
	Pickup mintCard = Pickup(renderer, images_dir.c_str(), 2, 3000.0f, 400.0f);



	/////////////////////////////////////////
	//           ***** LEVEL 2 *****
	/////////////////////////////////////////

	// bg
	SDL_Texture *l2bkgd = IMG_LoadTexture(renderer, (images_dir + "l2-bg.png").c_str());
	SDL_Rect l2bkgdRect;
	l2bkgdRect.x = 0;
	l2bkgdRect.y = 0;
	l2bkgdRect.w = 4096;
	l2bkgdRect.h = 768;
	float X_pos2 = 0.0f, Y_pos2 = 0.0f;


	// level 2 gate
	SDL_Texture *gate = IMG_LoadTexture(renderer, (images_dir + "gate.png").c_str());
	SDL_Rect gateRect;
	gateRect.x = 3900;
	gateRect.y = 400;
	gateRect.w = 80;
	gateRect.h = 325;
	float gateX_pos = 3900, gateY_pos = 400.0f;


	// key HUD

	SDL_Texture *keyB = IMG_LoadTexture(renderer, (images_dir + "key-empty.png").c_str());
	SDL_Texture *keyM = IMG_LoadTexture(renderer, (images_dir + "key-bar.png").c_str());
	SDL_Texture *keyF = IMG_LoadTexture(renderer, (images_dir + "key-top.png").c_str());

	SDL_Rect keyRect;
	keyRect.x = 725;
	keyRect.y = 16;
	keyRect.w = 284;
	keyRect.h = 91;

	SDL_Rect movingRect;
	movingRect.x = 725;
	movingRect.y = 16;
	movingRect.w = 284;
	movingRect.h = 91;

	Pickup key1 = Pickup(renderer, images_dir.c_str(), 3, 775.0f, 525.0f);
	Pickup key2 = Pickup(renderer, images_dir.c_str(), 4, 1500.0f, 525.0f);
	Pickup key3 = Pickup(renderer, images_dir.c_str(), 4, 2200.0f, 525.0f);
	Pickup key4 = Pickup(renderer, images_dir.c_str(), 4, 2900.0f, 525.0f);
	Pickup key5 = Pickup(renderer, images_dir.c_str(), 5, 3600.0f, 525.0f);

	float currentKeys = 0.0f;
	float maxKeys = 100.0f;





	// player
	Player player = Player(renderer, images_dir.c_str(), audio_dir.c_str(), 250.0, 500.0);

	// germs
	Germ germ1 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 800.0f, 500.0f);
	Germ germ2 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 1400.0f, 500.0f);
	Germ germ3 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 2000.0f, 500.0f);
	Germ germ4 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 2600.0f, 500.0f);
	Germ germ5 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 3200.0f, 500.0f);
	Germ germ6 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 3800.0f, 500.0f);

	// viruses
	Virus turret1 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 700.0f, 500.0f);
	Virus turret2 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 1400.0f, 500.0f);
	Virus turret3 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 2100.0f, 500.0f);
	Virus turret4 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 2800.0f, 500.0f);
	Virus turret5 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 3500.0f, 500.0f);





	while (!quit)
	{
		switch (gameState)
		{
		case MENU:

			Mix_PlayMusic(bgm, -1);

			alreadyOver = false;
			menu = true;

			while (menu)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (startOver)
								{
									Mix_PlayChannel(-1, pressedSound, 0);

									menu = false;
									gameState = LEVEL1;
									startOver = false;
								}

								if (backstoryOver)
								{
									Mix_PlayChannel(-1, pressedSound, 0);

									menu = false;
									gameState = BACKSTORY;
									backstoryOver = false;
								}

								if (instructionsOver)
								{
									Mix_PlayChannel(-1, pressedSound, 0);

									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								if (quitOver)
								{
									Mix_PlayChannel(-1, pressedSound, 0);
									SDL_Delay(1000);

									menu = false;
									quit = true;
									quitOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;

					}
				}


				UpdateBackground();
				UpdateCursor(deltaTime);

				startOver = SDL_HasIntersection(&activePos, &startPos);
				backstoryOver = SDL_HasIntersection(&activePos, &backstoryPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructionsPos);
				quitOver = SDL_HasIntersection(&activePos, &quitPos);

				if (startOver || backstoryOver || instructionsOver || quitOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if (!startOver && !backstoryOver && !instructionsOver && !quitOver)
				{
					alreadyOver = false;
				}



				SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					SDL_RenderCopy(renderer, ronaMenu, NULL, &ronaMenuPos);

					SDL_RenderCopy(renderer, title, NULL, &titlePos);


					if (startOver)
					{
						SDL_RenderCopy(renderer, startO, NULL, &startPos);
					}
					else {
						SDL_RenderCopy(renderer, startN, NULL, &startPos);
					}


					if (instructionsOver)
					{
						SDL_RenderCopy(renderer, instructionsO, NULL, &instructionsPos);
					}
					else {
						SDL_RenderCopy(renderer, instructionsN, NULL, &instructionsPos);
					}


					if (backstoryOver)
					{
						SDL_RenderCopy(renderer, backstoryO, NULL, &backstoryPos);
					}
					else {
						SDL_RenderCopy(renderer, backstoryN, NULL, &backstoryPos);
					}


					if (quitOver)
					{
						SDL_RenderCopy(renderer, quitO, NULL, &quitPos);
					}
					else {
						SDL_RenderCopy(renderer, quitN, NULL, &quitPos);
					}


					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				SDL_RenderPresent(renderer);
			}


			break;	// end main menu case





		case INSTRUCTIONS:
			alreadyOver = false;
			instructions = true;

			while (instructions)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;


				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						instructions = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (startOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									instructions = false;
									gameState = LEVEL1;
									startOver = false;
								}

								if (backstoryOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									instructions = false;
									gameState = BACKSTORY;
									backstoryOver = false;
								}

								if (menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									instructions = false;
									gameState = MENU;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);


				startOver = SDL_HasIntersection(&activePos, &startPosInst);
				backstoryOver = SDL_HasIntersection(&activePos, &backstoryPosInst);
				menuOver = SDL_HasIntersection(&activePos, &menuNPos);


				if (startOver || backstoryOver || menuOver || quitOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if (!startOver && !backstoryOver && !menuOver && !quitOver)
				{
					alreadyOver = false;
				}


				SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					SDL_RenderCopy(renderer, instructionsText, NULL, &instructionsTextPos);


					if (startOver)
					{
						SDL_RenderCopy(renderer, startO, NULL, &startPosInst);
					}
					else {
						SDL_RenderCopy(renderer, startN, NULL, &startPosInst);
					}


					if (backstoryOver)
					{
						SDL_RenderCopy(renderer, backstoryO, NULL, &backstoryPosInst);
					}
					else {
						SDL_RenderCopy(renderer, backstoryN, NULL, &backstoryPosInst);
					}


					if (menuOver)
					{
						SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
					}
					else {
						SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
					}


					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				SDL_RenderPresent(renderer);

			}
			break;	// end instructions case





		case BACKSTORY:
			alreadyOver = false;
			backstory = true;

			while (backstory)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;


				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						backstory = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (startOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									backstory = false;
									gameState = LEVEL1;
									startOver = false;
								}

								if (instructionsOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									backstory = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								if (menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									backstory = false;
									gameState = MENU;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);


				startOver = SDL_HasIntersection(&activePos, &startPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructionsPos);
				menuOver = SDL_HasIntersection(&activePos, &menuNPos);


				if (startOver || menuOver || instructionsOver || quitOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if (!startOver && !menuOver && !instructionsOver && !quitOver)
				{
					alreadyOver = false;
				}


				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				SDL_RenderCopy(renderer, backstoryText, NULL, &backstoryTextPos);


				if (startOver)
				{
					SDL_RenderCopy(renderer, startO, NULL, &startPos);
				}
				else {
					SDL_RenderCopy(renderer, startN, NULL, &startPos);
				}

				if (instructionsOver)
				{
					SDL_RenderCopy(renderer, instructionsO, NULL, &instructionsPos);
				}
				else {
					SDL_RenderCopy(renderer, instructionsN, NULL, &instructionsPos);
				}

				if (menuOver)
				{
					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				}
				else {
					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}


				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				SDL_RenderPresent(renderer);

			}
			break;	// end backstory case





		case LEVEL1:

			Mix_PlayMusic(bgl1, -1);

			alreadyOver = false;
			level1 = true;

			player.Reset();

			l1bkgdRect.x = 0;
			l2bkgdRect.x = 0;

			pos_X = 0;
			pos_Y = 0;
			X_pos = 0;
			X_pos = 0;
			X_pos2 = 0;

//			doorRect.x = 3670;

			// germs
			germ1 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 800.0f, 500.0f);
			germ2 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 1400.0f, 500.0f);
			germ3 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 2000.0f, 500.0f);
			germ4 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 2600.0f, 500.0f);
			germ5 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 3200.0f, 500.0f);
			germ6 = Germ(renderer, images_dir.c_str(), audio_dir.c_str(), 3800.0f, 500.0f);



			while (level1)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						level1 = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								level1 = false;
								gameState = WIN;
							}

							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								level1 = false;
								gameState = LOSE;
							}

							if (player.active)
								player.OnControllerButton(event.cbutton);
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						if (player.active)
							player.OnControllerAxis(event.caxis, deltaTime);
						break;
					}
				}


				if ((player.posRect.x >= 700 - player.posRect.w) && (player.posRect.x < 4096) && (player.xDir != 0)) {
					X_pos -= (player.speed) * deltaTime;
					doorX_pos -= (player.speed) * deltaTime;

					if ((l1bkgdRect.x > -3072)) {
						l1bkgdRect.x = (int)(X_pos + 0.5f);
						doorRect.x = (int)(doorX_pos + 0.5f);


						lilacCard.RonaMoveX(-player.speed, deltaTime);
						yellowCard.RonaMoveX(-player.speed, deltaTime);
						mintCard.RonaMoveX(-player.speed, deltaTime);

						germ1.GermMoveX(-player.speed, deltaTime);
						germ2.GermMoveX(-player.speed, deltaTime);
						germ3.GermMoveX(-player.speed, deltaTime);
						germ4.GermMoveX(-player.speed, deltaTime);
						germ5.GermMoveX(-player.speed, deltaTime);
						germ6.GermMoveX(-player.speed, deltaTime);
					}
					else {

						X_pos = l1bkgdRect.x;
						doorX_pos = doorRect.x;
					}
				}

				if ((player.posRect.x <= 224) && (player.posRect.x < 4096) && (player.xDir != 0)) {
					X_pos += (player.speed) * deltaTime;
					doorX_pos += (player.speed) * deltaTime;

					if ((l1bkgdRect.x < 0)) {
						l1bkgdRect.x = (int)(X_pos + 0.5f);
						doorRect.x = (int)(doorX_pos + 0.5f);

						lilacCard.RonaMoveX(player.speed, deltaTime);
						yellowCard.RonaMoveX(player.speed, deltaTime);
						mintCard.RonaMoveX(player.speed, deltaTime);

						germ1.GermMoveX(player.speed, deltaTime);
						germ2.GermMoveX(player.speed, deltaTime);
						germ3.GermMoveX(player.speed, deltaTime);
						germ4.GermMoveX(player.speed, deltaTime);
						germ5.GermMoveX(player.speed, deltaTime);
						germ6.GermMoveX(player.speed, deltaTime);
					}
					else {

						X_pos = l1bkgdRect.x;
						doorX_pos = doorRect.x;
					}
				}


				UpdateBackground();

				if (player.active)
					player.Update(deltaTime, renderer);

				germ1.Update(deltaTime, player.posRect);
				germ2.Update(deltaTime, player.posRect);
				germ3.Update(deltaTime, player.posRect);
				germ4.Update(deltaTime, player.posRect);
				germ5.Update(deltaTime, player.posRect);
				germ6.Update(deltaTime, player.posRect);



				for (int i = 0; i < player.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&germ1.germRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						if (germ1.active == true)
						{
							germ1.RemoveHealth();
						}
						break;
					}

					if (SDL_HasIntersection(&germ2.germRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						if (germ2.active == true)
						{
							germ2.RemoveHealth();
						}
						break;
					}

					if (SDL_HasIntersection(&germ3.germRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						if (germ3.active == true)
						{
							germ3.RemoveHealth();
						}
						break;
					}

					if (SDL_HasIntersection(&germ4.germRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						if (germ4.active == true)
						{
							germ4.RemoveHealth();
						}
						break;
					}

					if (SDL_HasIntersection(&germ5.germRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						if (germ5.active == true)
						{
							germ5.RemoveHealth();
						}
						break;
					}

					if (SDL_HasIntersection(&germ6.germRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						if (germ6.active == true)
						{
							germ6.RemoveHealth();
						}
						break;
					}
				}



				if (SDL_HasIntersection(&player.posRect, &lilacCard.cardRect)) {
					haveLilac = true;
					lilacCard.active = false;
					lilacCard.cardRect.x = -5000;
					lilacCard.cardRect.y = -5000;
				}

				if (SDL_HasIntersection(&player.posRect, &yellowCard.cardRect)) {
					haveYellow = true;
					yellowCard.active = false;
					yellowCard.cardRect.x = -5000;
					yellowCard.cardRect.y = -5000;
				}

				if (SDL_HasIntersection(&player.posRect, &mintCard.cardRect)) {
					haveMint = true;
					mintCard.active = false;
					mintCard.cardRect.x = -5000;
					mintCard.cardRect.y = -5000;
				}



				if (SDL_HasIntersection(&player.posRect, &germ1.germRect))
				{
					player.GermHit();
				}

				if (SDL_HasIntersection(&player.posRect, &germ2.germRect))
				{
					player.GermHit();
				}

				if (SDL_HasIntersection(&player.posRect, &germ3.germRect))
				{
					player.GermHit();
				}

				if (SDL_HasIntersection(&player.posRect, &germ4.germRect))
				{
					player.GermHit();
				}

				if (SDL_HasIntersection(&player.posRect, &germ5.germRect))
				{
					player.GermHit();
				}

				if (SDL_HasIntersection(&player.posRect, &germ6.germRect))
				{
					player.GermHit();
				}

				if (player.playerHealth <= 0)
				{
					level1 = false;
					gameState = LOSE;
				}



				if ((SDL_HasIntersection(&player.posRect, &doorRect)) && haveLilac && haveMint && haveYellow)
				{
					level1 = false;
					gameState = LEVEL2;
				}



				SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, l1bkgd, NULL, &l1bkgdRect);

					SDL_RenderCopy(renderer, door, NULL, &doorRect);

					SDL_RenderCopy(renderer, Cardbkgd, NULL, &CardbkgdRect);

					if (haveLilac)
						SDL_RenderCopy(renderer, lilac, NULL, &CardbkgdRect);
					if (haveYellow)
						SDL_RenderCopy(renderer, yellow, NULL, &CardbkgdRect);
					if (haveMint)
						SDL_RenderCopy(renderer, mint, NULL, &CardbkgdRect);

					if (lilacCard.active)
						lilacCard.Draw(renderer);
					if (yellowCard.active)
						yellowCard.Draw(renderer);
					if (mintCard.active)
						mintCard.Draw(renderer);


					player.Draw(renderer);

					germ1.Draw(renderer);
					germ2.Draw(renderer);
					germ3.Draw(renderer);
					germ4.Draw(renderer);
					germ5.Draw(renderer);
					germ6.Draw(renderer);

				SDL_RenderPresent(renderer);

			}
			break;	// end level 1 case





		case LEVEL2:

			Mix_PlayMusic(bgl2, -1);

			alreadyOver = false;
			level2 = true;

			player.Reset();

			l1bkgdRect.x = 0;
			l2bkgdRect.x = 0;
			gateRect.x = 3900;

			pos_X = 0;
			X_pos = 0;
			X_pos2 = 0;

			turret1 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 700.0f, 500.0f);
			turret2 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 1400.0f, 500.0f);
			turret3 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 2100.0f, 500.0f);
			turret4 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 2800.0f, 500.0f);
			turret5 = Virus(renderer, images_dir.c_str(), audio_dir.c_str(), 3500.0f, 500.0f);



			while (level2)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						level2 = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								level2 = false;
								gameState = WIN;
							}

							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								level2 = false;
								gameState = LOSE;
							}

							if (player.active)
								player.OnControllerButton(event.cbutton);
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						if (player.active)
							player.OnControllerAxis(event.caxis, deltaTime);
						break;
					}
				}


				if ((player.posRect.x >= 750 - player.posRect.w) && (player.posRect.x < 4096) && (player.xDir != 0)) {
					X_pos2 -= (player.speed) * deltaTime;
					gateX_pos -= (player.speed) * deltaTime;

					if ((l2bkgdRect.x > -3072)) {
						l2bkgdRect.x = (int)(X_pos2 + 0.5f);
						gateRect.x = (int)(gateX_pos + 0.5f);

						key1.RonaMoveX(-player.speed, deltaTime);
						key2.RonaMoveX(-player.speed, deltaTime);
						key3.RonaMoveX(-player.speed, deltaTime);
						key4.RonaMoveX(-player.speed, deltaTime);
						key5.RonaMoveX(-player.speed, deltaTime);

						turret1.PlayerMoveX(-player.speed, deltaTime);
						turret2.PlayerMoveX(-player.speed, deltaTime);
						turret3.PlayerMoveX(-player.speed, deltaTime);
						turret4.PlayerMoveX(-player.speed, deltaTime);
						turret5.PlayerMoveX(-player.speed, deltaTime);
					}
					else {

						X_pos2 = l2bkgdRect.x;
						gateX_pos = gateRect.x;
					}
				}

				if ((player.posRect.x <= 224) && (player.posRect.x < 4096) && (player.xDir != 0)) {
					X_pos2 += (player.speed) * deltaTime;
					gateX_pos += (player.speed) * deltaTime;


					if ((l2bkgdRect.x < 0)) {
						l2bkgdRect.x = (int)(X_pos2 + 0.5f);
						gateRect.x = (int)(gateX_pos + 0.5f);

						key1.RonaMoveX(player.speed, deltaTime);
						key2.RonaMoveX(player.speed, deltaTime);
						key3.RonaMoveX(player.speed, deltaTime);
						key4.RonaMoveX(player.speed, deltaTime);
						key5.RonaMoveX(player.speed, deltaTime);

						turret1.PlayerMoveX(player.speed, deltaTime);
						turret2.PlayerMoveX(player.speed, deltaTime);
						turret3.PlayerMoveX(player.speed, deltaTime);
						turret4.PlayerMoveX(player.speed, deltaTime);
						turret5.PlayerMoveX(player.speed, deltaTime);
					}
					else {

						X_pos2 = l2bkgdRect.x;
						gateX_pos = gateRect.x;
					}
				}


				UpdateBackground();

				if (player.active)
					player.Update(deltaTime, renderer);

				turret1.Update(deltaTime, player.posRect);
				turret2.Update(deltaTime, player.posRect);
				turret3.Update(deltaTime, player.posRect);
				turret4.Update(deltaTime, player.posRect);
				turret5.Update(deltaTime, player.posRect);



				// collision for viruses shooting the player
				for (int i = 0; i < turret1.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player.posRect, &turret1.bulletList[i].posRect))
					{
						turret1.bulletList[i].Reset();
						player.BulletHit();
						break;
					}
				}

				for (int i = 0; i < turret2.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player.posRect, &turret2.bulletList[i].posRect))
					{
						turret2.bulletList[i].Reset();
						player.BulletHit();
						break;
					}
				}

				for (int i = 0; i < turret3.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player.posRect, &turret3.bulletList[i].posRect))
					{
						turret3.bulletList[i].Reset();
						player.BulletHit();
						break;
					}
				}

				for (int i = 0; i < turret4.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player.posRect, &turret4.bulletList[i].posRect))
					{
						turret4.bulletList[i].Reset();
						player.BulletHit();
						break;
					}
				}

				for (int i = 0; i < turret5.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player.posRect, &turret5.bulletList[i].posRect))
					{
						turret5.bulletList[i].Reset();
						player.BulletHit();
						break;
					}
				}

				if (player.playerHealth <= 0)
				{
					level2 = false;
					gameState = LOSE;
				}

				if ((SDL_HasIntersection(&player.posRect, &gateRect)) && !key1.active && 
					!key2.active && !key3.active && !key4.active && !key5.active)
				{
					level2 = false;
					gameState = WIN;
				}




				// collision for player shooting turrets
				for (int i = 0; i < player.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&turret1.baseRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						turret1.RemoveHealth();
						break;
					}

					if (SDL_HasIntersection(&turret2.baseRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						turret2.RemoveHealth();
						break;
					}

					if (SDL_HasIntersection(&turret3.baseRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						turret3.RemoveHealth();
						break;
					}

					if (SDL_HasIntersection(&turret4.baseRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						turret4.RemoveHealth();
						break;
					}

					if (SDL_HasIntersection(&turret5.baseRect, &player.bulletList[i].posRect))
					{
						player.bulletList[i].Reset();
						turret5.RemoveHealth();
						break;
					}
				}



				if (SDL_HasIntersection(&player.posRect, &key1.cardRect))
				{
					key1.active = false;
					key1.cardRect.x = -5000;
					key1.cardRect.y = -5000;
					currentKeys += 100 / 5;
				}

				if (SDL_HasIntersection(&player.posRect, &key2.cardRect))
				{
					key2.active = false;
					key2.cardRect.x = -5000;
					key2.cardRect.y = -5000;
					currentKeys += 100 / 5;
				}

				if (SDL_HasIntersection(&player.posRect, &key3.cardRect))
				{
					key3.active = false;
					key3.cardRect.x = -5000;
					key3.cardRect.y = -5000;
					currentKeys += 100 / 5;
				}

				if (SDL_HasIntersection(&player.posRect, &key4.cardRect))
				{
					key4.active = false;
					key4.cardRect.x = -5000;
					key4.cardRect.y = -5000;
					currentKeys += 100 / 5;
				} 

				if (SDL_HasIntersection(&player.posRect, &key5.cardRect))
				{
					key5.active = false;
					key5.cardRect.x = -5000;
					key5.cardRect.y = -5000;
					currentKeys += 100 / 5;
				}

				movingRect.w = currentKeys / maxKeys * 284;



				SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, l2bkgd, NULL, &l2bkgdRect);

					SDL_RenderCopy(renderer, gate, NULL, &gateRect);

					SDL_RenderCopy(renderer, keyB, NULL, &keyRect);
					SDL_RenderCopy(renderer, keyM, NULL, &movingRect);
					SDL_RenderCopy(renderer, keyF, NULL, &keyRect);


					if (key1.active)
						key1.Draw(renderer);
					if (key2.active)
						key2.Draw(renderer);
					if (key3.active)
						key3.Draw(renderer);
					if (key4.active)
						key4.Draw(renderer);
					if (key5.active)
						key5.Draw(renderer);


					player.Draw(renderer);

					turret1.Draw(renderer);
					turret2.Draw(renderer);
					turret3.Draw(renderer);
					turret4.Draw(renderer);
					turret5.Draw(renderer);

				SDL_RenderPresent(renderer);

			}
			break;	// end level 2 case





		case WIN:
			alreadyOver = false;
			win = true;

			haveMint = false;
			haveLilac = false;
			haveYellow = false;

			mintCard.active = true;
			lilacCard.active = true;
			yellowCard.active = true;
			

			while (win)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									win = false;
									gameState = MENU;
									menuOver = false;
								}

								if (level1Over) {
									Mix_PlayChannel(-1, pressedSound, 0);

									win = false;
									gameState = LEVEL1;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;

					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &menuWPos);
				level1Over = SDL_HasIntersection(&activePos, &playNPos);

				if (menuOver || level1Over)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if (!menuOver && !level1Over)
				{
					alreadyOver = false;
				}


				SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					SDL_RenderCopy(renderer, winGraphic, NULL, &winGraphicPos);

					SDL_RenderCopy(renderer, winText, NULL, &winTextPos);


					if (menuOver) {

						SDL_RenderCopy(renderer, menuO, NULL, &menuWPos);
					}
					else {

						SDL_RenderCopy(renderer, menuN, NULL, &menuWPos);
					}

					if (level1Over) {

						SDL_RenderCopy(renderer, playO, NULL, &playNPos);
					}
					else {

						SDL_RenderCopy(renderer, playN, NULL, &playNPos);
					}


					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				SDL_RenderPresent(renderer);

			}
			break;	// end win case





		case LOSE:

			Mix_PlayMusic(bglose, -1);

			alreadyOver = false;
			lose = true;

			haveMint = false;
			haveLilac = false;
			haveYellow = false;

			key1.active = false;
			key2.active = false;
			key3.active = false;
			key4.active = false;
			key5.active = false;

			while (lose)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (level1Over) {
									Mix_PlayChannel(-1, pressedSound, 0);

									lose = false;
									gameState = LEVEL1;
									level1Over = false;
								}

								if (menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									lose = false;
									gameState = MENU;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;

					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &menuWPos);
				level1Over = SDL_HasIntersection(&activePos, &playNPos);

				if (menuOver || level1Over)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if (!menuOver && !level1Over)
				{
					alreadyOver = false;
				}


				SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					SDL_RenderCopy(renderer, bgFilter, NULL, &bgFilterPos);

					SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);


					if (menuOver) {

						SDL_RenderCopy(renderer, menuO, NULL, &menuWPos);
					}
					else {

						SDL_RenderCopy(renderer, menuN, NULL, &menuWPos);
					}


					if (level1Over) {

						SDL_RenderCopy(renderer, playO, NULL, &playNPos);
					}
					else {

						SDL_RenderCopy(renderer, playN, NULL, &playNPos);
					}


					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				SDL_RenderPresent(renderer);

			}
			break;	// end lose case
		}
	}



	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
