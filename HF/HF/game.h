#ifndef GAME_HH
#define GAME_HH

#include "SDL.h"
#include <string>

class Font;
class Background;
class Plane;

enum GameStates { GS_INTRO, GS_PLAYON, GS_ENDING, GS_END, GS_QUIT };

class Game {
	SDL_Surface *screen;
	SDL_Surface *pauseSprite;

	Uint32 frameCnt;
	Uint32 tickCnt;
	Uint32 sdlTicks;
	Uint32 timePauseOn;
	Uint32 timeLastUpdate;
	Uint32 gameActRuntime;
	Uint32 gameLength;
	GameStates gameState;
	bool paused;

	Font *fontTime;
	int fontSizeTime;

	Background *background;
	Plane* m_pPlane;

public:

	Game();
	~Game();

	void run();

private:
	void initNewGame();
	void playOn();
	void handleEventsPlayOn();
	void pause();
	void updateGameState();
	void drawPlayOn();
	void drawBackground();
	void drawTime();
	void drawDebugInfos();
	void drawPaused();

};

#endif //#ifndef GAME_HH
