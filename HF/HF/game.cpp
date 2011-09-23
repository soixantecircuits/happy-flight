using namespace std;

#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "SDL.h"
#include "surfaceDB.h"
#include "video.h"
#include "font.h"
#include "background.h"
#include "PrefsManager.h"
#include "plane.h"

bool debug;
bool scrollingOn;
float scrollspeed, movespeed;
float actBackgroundPos;

//keys
bool leftDown;
bool rightDown;
bool F5down;
bool F6down;
bool F7down;
bool F8down;

template<typename T> std::string asString( const T& obj )
{
	std::ostringstream t;
	t << obj;
	std::string res(t.str());
	return res;
}

Game::Game()
{
	PrefsManager::GetInstance()->LoadPrefs("prefs.ini");
	scrollspeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
	movespeed = (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" );
	debug = PrefsManager::GetInstance()->GetValue("DEBUG")==0?false:true;

	videoserver = new Video();
	screen = 0;
	screen = videoserver->init();

	// needed for calculating fps
	frameCnt = 0;
	tickCnt = 0;

	gameState = GS_INTRO;
	paused = true;
	sdlTicks = SDL_GetTicks();

	pauseSprite = surfaceDB.loadSurface( "../../resources/imgs/paused.png" );
	fontTime = new Font( "../../resources/imgs/font-20red.png" );
	fontSizeTime = fontTime->getCharWidth();

	scrollingOn = true;
	background = new Background();
	background->generateBackground( PrefsManager::GetInstance()->GetValue("BKG_LENGTH") );

	m_pPlane = NULL;
}

Game::~Game()
{
	if( m_pPlane ) delete m_pPlane;
	if( videoserver ) delete videoserver;
}

void Game::initNewGame()
{
	if( m_pPlane ) delete m_pPlane;
	m_pPlane = new Plane();
	m_pPlane->setMaxVel( movespeed );

	actBackgroundPos = 0;
	gameActRuntime = 0;
	paused = true;

	timeLastUpdate = SDL_GetTicks();
	timePauseOn = SDL_GetTicks();
}

void Game::run(){
	while( gameState != GS_QUIT )
	{
		switch (gameState)
		{
			case GS_INTRO: 
			{
				gameState = GS_PLAYON;
				//intro->run( gameState );
				//break;
			}
			case GS_ENDING:
			{
				break;
			}
			case GS_END:
			{
				break;
			}
			case GS_PLAYON: 
			{
				initNewGame();
				playOn();
				break;
			}
			default: break;
		}
	}
}


/*********************************/
/**** PlayOn *********************/
/*********************************/

void Game::playOn()
{
	int A = SDL_GetTicks();
	frameCnt = 0;
	tickCnt = 0;
	cout << "frameCnt: " << frameCnt << "  tickCnt: " << tickCnt << "  SDL_GetTicks()=" <<  A << endl;

	while( gameState == GS_PLAYON || gameState == GS_ENDING )
	{
		handleEventsPlayOn();
		if( !paused ) 
		{
			updateGameState();
		}
		drawPlayOn();
	}

	int B = SDL_GetTicks();
	cout << "frameCnt: " << frameCnt << "  tickCnt: " << tickCnt << "  SDL_GetTicks()=" <<  B << endl;
	cout << "Miliseconds: " << B-A << endl;
	cout << "Frames/sec : " << (float)frameCnt / ((float)(B-A) / 1000.0) << endl;
	cout << "ms/Frame   : " << (float)tickCnt / (float)frameCnt << endl;
}


void Game::pause()
{
	if (paused)
	{
		Uint32 timePaused = SDL_GetTicks() - timePauseOn;
		timeLastUpdate += timePaused;
	}
	else
	{
		timePauseOn = SDL_GetTicks();
	}
	paused = !paused;
}


void Game::handleEventsPlayOn()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_p )
					pause();
				else
					if (paused) pause();
					//m_pPlane->handlePlayerEvent(input.translate(event), input.isPressed(event));

				switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
					{
						leftDown = true;
						break;
					}
				case SDLK_RIGHT:
					{
						rightDown = true;
						break;
					}
				case SDLK_F5:
					{
						F5down = true;
						break;
					}
				case SDLK_F6:
					{
						F6down = true;
						break;
					}
				case SDLK_F7:
					{
						F7down = true;
						break;
					}
				case SDLK_F8:
					{
						F8down = true;
						break;
					}
				default: break;
				}
				break;
			}
			case SDL_KEYUP:
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
					{
						leftDown = false;
						break;
					}
				case SDLK_RIGHT:
					{
						rightDown = false;
						break;
					}
				case SDLK_F5:
					{
						F5down = false;
						break;
					}
				case SDLK_F6:
					{
						F6down = false;
						break;
					}
				case SDLK_F7:
					{
						F7down = false;
						break;
					}
				case SDLK_F8:
					{
						F8down = false;
						break;
					}
				case SDLK_F9:
					{
						break;
					}
				case SDLK_F10:
					{
						break;
					}
				case SDLK_d:
					{
						debug = !debug;
						break;
					}
				case SDLK_f:
					{
						videoserver->toggleFullscreen();
						break;
					}
				case SDLK_r:
					{
						gameState = GS_INTRO;
						break;
					}
				case SDLK_F12:
					{
						PrefsManager::GetInstance()->LoadPrefs("prefs.ini");
						scrollspeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
						debug = PrefsManager::GetInstance()->GetValue("DEBUG")==0?false:true;
						m_pPlane->setMaxVel( (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" ) );
						break;
					}
				case SDLK_ESCAPE:
					{
						gameState = GS_QUIT;
						break;
					}
				default: break;
				}
				break;
			}
			case SDL_QUIT:
				{
					gameState = GS_QUIT;
					break;
				}
			default: break;
		}
	}
	if( F5down )
	{
		scrollspeed--;
		if( scrollspeed < 0 )
			scrollspeed = 0;
	}
	if( F6down )
		scrollspeed++;

	m_pPlane->left = leftDown;
	m_pPlane->right = rightDown;
	
}


// what to do in one tick
void Game::updateGameState()
{
	int dT = (SDL_GetTicks() - timeLastUpdate);
	timeLastUpdate += dT;
	gameActRuntime += dT;

	if ( scrollingOn ) actBackgroundPos -= (float)( scrollspeed * dT / 1000.0 );

	scrollspeed += (float)dT/200;
	movespeed += (float)dT/200;

	m_pPlane->setMaxVel( movespeed );
	m_pPlane->move( dT );
	m_pPlane->pickUpItems();
}

void Game::drawPlayOn()
{
	drawBackground();
	//m_pPlane->drawShadows(screen);
	m_pPlane->drawPlane(screen);
	//m_pPlane->drawStats(screen);

	if( debug )
	{
		if( gameState == GS_PLAYON )
			drawTime();
		drawDebugInfos();
	}

	if (paused) drawPaused();

	SDL_Flip( screen );

	frameCnt++;
}

void Game::drawBackground()
{
	background->draw(screen, (int) (actBackgroundPos + 0.5) );
}


void Game::drawTime()
{
	int timeToDraw;
	timeToDraw = gameActRuntime / 1000;
	if ( timeToDraw > 0 )
	{
		int digitCnt = 1;
		int i=1;
		while ( timeToDraw >= i*10 )
		{
			digitCnt++;
			i *= 10;
		}
		fontTime->drawInt(screen, (screen->w / 2) - (fontSizeTime * digitCnt) / 2, 5, timeToDraw, digitCnt, 0);
	}
}

void Game::drawDebugInfos()
{
	string debuginfo =  "Scroll speed : " + asString( (int)scrollspeed );
	fontTime->drawStr( screen, 10, 10, debuginfo );

	debuginfo =  "Move speed : " + asString( (int)movespeed );
	fontTime->drawStr( screen, 10, 30, debuginfo );
}

void Game::drawPaused()
{
	SDL_Rect r;
	r.x = screen->w/2 - pauseSprite->w/2;
	r.y = screen->h/2 - pauseSprite->h/2;
	r.w = pauseSprite->w;
	r.h = pauseSprite->h;
	SDL_BlitSurface( pauseSprite, 0, screen, &r );
}

