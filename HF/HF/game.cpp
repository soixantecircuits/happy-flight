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
#include "items.h"

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
	m_fAcceleration = (float)PrefsManager::GetInstance()->GetValue( "ACCELERATION" );
	m_bDebug = PrefsManager::GetInstance()->GetValue("DEBUG")==0?false:true;

	g_pVideoserver = new Video();
	screen = 0;
	screen = g_pVideoserver->Init();

	m_bLeftDown = false;
	m_bRightDown = false;

	// needed for calculating fps
	m_iFrameCnt = 0;
	m_iTickCnt = 0;

	m_eGameState = GS_INTRO;
	m_bPaused = true;
	m_iSdlTicks = SDL_GetTicks();

	pauseSprite = surfaceDB.LoadSurface( "../../resources/imgs/paused.png" );
	m_pDebugFont = new Font( "../../resources/imgs/font-20red.png" );
	m_iDebugFontSize = m_pDebugFont->GetCharWidth();

	m_bScrolling = true;
	m_pBackground = new Background();
	m_pBackground->GenerateBackground( PrefsManager::GetInstance()->GetValue("BKG_LENGTH") );

	m_pPlane = NULL;
}

Game::~Game()
{
	if( m_pPlane ) delete m_pPlane;
	if( g_pVideoserver ) delete g_pVideoserver;
	//if( m_pItems ) delete m_pItems;
}

void Game::InitNewGame()
{
	m_fScrollSpeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
	m_fMoveSpeed = (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" );

	if( m_pPlane ) delete m_pPlane;
	m_pPlane = new Plane();
	m_pPlane->SetMaxVel( m_fMoveSpeed );

	m_fActBackgoundPos = 0;
	m_iGameActRuntime = 0;
	m_bPaused = true;

	m_iTimeLastUpdate = SDL_GetTicks();
	m_iTimePauseOn = SDL_GetTicks();
}

void Game::Run(){
	while( m_eGameState != GS_QUIT )
	{
		switch (m_eGameState)
		{
			case GS_INTRO: 
			{
				m_eGameState = GS_PLAYON;
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
				InitNewGame();
				PlayOn();
				break;
			}
			default: break;
		}
	}
}


/*********************************/
/**** PlayOn *********************/
/*********************************/

void Game::PlayOn()
{
	int A = SDL_GetTicks();
	m_iFrameCnt = 0;
	m_iTickCnt = 0;
	cout << "frameCnt: " << m_iFrameCnt << "  tickCnt: " << m_iTickCnt << "  SDL_GetTicks()=" <<  A << endl;

	while( m_eGameState == GS_PLAYON || m_eGameState == GS_ENDING )
	{
		HandleEventsPlayOn();
		if( !m_bPaused ) 
		{
			UpdateGameState();
		}
		DrawPlayOn();
	}

	int B = SDL_GetTicks();
	cout << "frameCnt: " << m_iFrameCnt << "  tickCnt: " << m_iTickCnt << "  SDL_GetTicks()=" <<  B << endl;
	cout << "Miliseconds: " << B-A << endl;
	cout << "Frames/sec : " << (float)m_iFrameCnt / ((float)(B-A) / 1000.0) << endl;
	cout << "ms/Frame   : " << (float)m_iTickCnt / (float)m_iFrameCnt << endl;
}


void Game::Pause()
{
	if (m_bPaused)
	{
		Uint32 timePaused = SDL_GetTicks() - m_iTimePauseOn;
		m_iTimeLastUpdate += timePaused;
	}
	else
	{
		m_iTimePauseOn = SDL_GetTicks();
	}
	m_bPaused = !m_bPaused;
}


void Game::HandleEventsPlayOn()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_p )
					Pause();
				else
					if (m_bPaused) Pause();
					//m_pPlane->handlePlayerEvent(input.translate(event), input.isPressed(event));

				switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
					{
						m_bLeftDown = true;
						break;
					}
				case SDLK_RIGHT:
					{
						m_bRightDown = true;
						break;
					}
				case SDLK_F5:
					{
						m_bF5down = true;
						break;
					}
				case SDLK_F6:
					{
						m_bF6down = true;
						break;
					}
				case SDLK_F7:
					{
						m_bF7down = true;
						break;
					}
				case SDLK_F8:
					{
						m_bF8down = true;
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
						m_bLeftDown = false;
						break;
					}
				case SDLK_RIGHT:
					{
						m_bRightDown = false;
						break;
					}
				case SDLK_F5:
					{
						m_bF5down = false;
						break;
					}
				case SDLK_F6:
					{
						m_bF6down = false;
						break;
					}
				case SDLK_F7:
					{
						m_bF7down = false;
						break;
					}
				case SDLK_F8:
					{
						m_bF8down = false;
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
						m_bDebug = !m_bDebug;
						break;
					}
				case SDLK_f:
					{
						g_pVideoserver->ToggleFullscreen();
						break;
					}
				case SDLK_r:
					{
						m_eGameState = GS_INTRO;
						break;
					}
				case SDLK_F12:
					{
						PrefsManager::GetInstance()->LoadPrefs("prefs.ini");
						m_fScrollSpeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
						m_bDebug = PrefsManager::GetInstance()->GetValue("DEBUG")==0?false:true;
						m_pPlane->SetMaxVel( (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" ) );
						break;
					}
				case SDLK_ESCAPE:
					{
						m_eGameState = GS_QUIT;
						break;
					}
				default: break;
				}
				break;
			}
			case SDL_QUIT:
				{
					m_eGameState = GS_QUIT;
					break;
				}
			default: break;
		}
	}
	if( m_bF5down )
	{
		m_fScrollSpeed--;
		if( m_fScrollSpeed < 0 )
			m_fScrollSpeed = 0;
	}
	if( m_bF6down )
		m_fScrollSpeed++;
}


// what to do in one tick
void Game::UpdateGameState()
{
	int dT = (SDL_GetTicks() - m_iTimeLastUpdate);
	m_iTimeLastUpdate += dT;
	m_iGameActRuntime += dT;

	if ( m_bScrolling ) m_fActBackgoundPos -= (float)( m_fScrollSpeed * dT / 1000.0 );

	m_fScrollSpeed += (float)dT/m_fAcceleration;
	m_fMoveSpeed += (float)dT/m_fAcceleration;

	if( m_bLeftDown )
		m_pPlane->GoLeft();
	else if( m_bRightDown )
		m_pPlane->GoRight();

	m_pPlane->SetMaxVel( m_fMoveSpeed );
	m_pPlane->Move( dT );
	m_pPlane->PickUpItems();
}

void Game::DrawPlayOn()
{
	DrawBackground();
	//m_pPlane->drawShadows(screen);
	m_pPlane->DrawPlane(screen);
	//m_pPlane->drawStats(screen);

	if( m_bDebug )
	{
		if( m_eGameState == GS_PLAYON )
			DrawTime();
		DrawDebugInfos();
	}

	if (m_bPaused) DrawPaused();

	SDL_Flip( screen );

	m_iFrameCnt++;
}

void Game::DrawBackground()
{
	m_pBackground->Draw(screen, (int) (m_fActBackgoundPos + 0.5) );
}


void Game::DrawTime()
{
	int timeToDraw;
	timeToDraw = m_iGameActRuntime / 1000;
	if ( timeToDraw > 0 )
	{
		int digitCnt = 1;
		int i=1;
		while ( timeToDraw >= i*10 )
		{
			digitCnt++;
			i *= 10;
		}
		m_pDebugFont->DrawInt(screen, (screen->w / 2) - (m_iDebugFontSize * digitCnt) / 2, 5, timeToDraw, digitCnt, 0);
	}
}

void Game::DrawDebugInfos()
{
	string debuginfo =  "Scroll speed : " + asString( (int)m_fScrollSpeed );
	m_pDebugFont->DrawStr( screen, 10, 10, debuginfo );

	debuginfo =  "Move speed : " + asString( (int)m_fMoveSpeed );
	m_pDebugFont->DrawStr( screen, 10, 30, debuginfo );
}

void Game::DrawPaused()
{
	SDL_Rect r;
	r.x = screen->w/2 - pauseSprite->w/2;
	r.y = screen->h/2 - pauseSprite->h/2;
	r.w = pauseSprite->w;
	r.h = pauseSprite->h;
	SDL_BlitSurface( pauseSprite, 0, screen, &r );
}

