#ifndef GAME_HH
#define GAME_HH

#include "SDL.h"
#include <string>

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

class MyFont;
class Background;
class Plane;
class Items;
class Score;
class ofxOscReceiver;

enum GameStates { GS_LOADING, GS_INTRO, GS_PLAYON, GS_QUIT };

class Game {
	SDL_Surface *m_pScreen;
	int m_iPauseSprite;
	SDL_Surface* m_pPauseSprite;

	Uint32 m_iFrameCnt;
	Uint32 m_iSdlTicks;
	Uint32 m_iTimePauseOn;
	Uint32 m_iTimeLastUpdate;
	Uint32 m_iGameActRuntime;
	GameStates m_eGameState;
	bool m_bPaused;

	int iMs;

	MyFont *m_pDebugFont;
	int m_iDebugFontSize;

	Background *m_pBackground;
	Plane* m_pPlane;
	Items* m_pItems;
	Score* m_pScore;
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
	bool m_bOSCLeft;
	bool m_bOSCRight;
	bool m_bF5down;
	bool m_bF6down;
	bool m_bF7down;
	bool m_bF8down;

	bool m_bEnd;
	
	ofxOscReceiver	*m_pReceiver;

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
	void HandleOSCEventsPlayOn();
	void Pause();
	void UpdateGameState();
	void DrawPlayOn();
	void DrawBackground();
	void DrawDebugInfos();
	void DrawPaused();
	void LoadResources();
	void DrawLoading( bool bLoaded = false );
};

#endif //#ifndef GAME_HH
