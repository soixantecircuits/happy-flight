#ifndef GAME_HH
#define GAME_HH

#include "SDL.h"
#include <string>

class Font;
class Background;
class Plane;
class Items;

enum GameStates { GS_INTRO, GS_PLAYON, GS_ENDING, GS_END, GS_QUIT };

class Game {
	SDL_Surface *m_pScreen;
	SDL_Surface *m_pPauseSprite;

	Uint32 m_iFrameCnt;
	Uint32 m_iTickCnt;
	Uint32 m_iSdlTicks;
	Uint32 m_iTimePauseOn;
	Uint32 m_iTimeLastUpdate;
	Uint32 m_iGameActRuntime;
	GameStates m_eGameState;
	bool m_bPaused;

	Font *m_pDebugFont;
	int m_iDebugFontSize;

	Background *m_pBackground;
	Plane* m_pPlane;
	Items* m_pItems;
	int m_iScore;

	bool m_bDebug;
	bool m_bScrolling;
	float m_fScrollSpeed;
	float m_fMoveSpeed;
	float m_fAcceleration;
	float m_fActBackgoundPos;

	//keys
	bool m_bLeftDown;
	bool m_bRightDown;
	bool m_bF5down;
	bool m_bF6down;
	bool m_bF7down;
	bool m_bF8down;

public:

	Game();
	~Game();

	void Run();

	void PickUpCoin();
	void EnterCloud();
	void EnterThunder();

private:
	void InitNewGame();
	void PlayOn();
	void HandleEventsPlayOn();
	void Pause();
	void UpdateGameState();
	void DrawPlayOn();
	void DrawBackground();
	void DrawTime();
	void DrawDebugInfos();
	void DrawPaused();
};

#endif //#ifndef GAME_HH
