using namespace std;

#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "SDL.h"
#include "TextureManager.h"
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

	m_pScreen = Video::GetInstance()->Init();

	m_bLeftDown = false;
	m_bRightDown = false;
	m_bF5down = false;
	m_bF6down = false;
	m_bF7down = false;
	m_bF8down = false;

	m_iScore = 0;

	// needed for calculating fps
	m_iFrameCnt = 0;

	m_eGameState = GS_INTRO;
	m_bPaused = true;
	m_iSdlTicks = SDL_GetTicks();

	m_iPauseSprite = TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/paused.png" );
	m_pPauseSprite = TextureManager::GetInstance()->GetTextureById( m_iPauseSprite );
	m_pDebugFont = new Font( "../../resources/imgs/font-20red.png" );
	m_iDebugFontSize = m_pDebugFont->GetCharWidth();

	LoadResources();

	m_bScrolling = true;
	m_pBackground = new Background();
	m_pBackground->GenerateBackground( PrefsManager::GetInstance()->GetValue("BKG_LENGTH") );

	m_pItems = NULL;
	m_pPlane = NULL;
}

Game::~Game()
{
	if( m_pPlane ) delete m_pPlane;
	if( m_pItems ) delete m_pItems;
	TextureManager::GetInstance()->Delete();
	Video::GetInstance()->Delete();
}

void Game::InitNewGame()
{
	m_iScore = 0;
	m_iEnding = 0;
	m_fScrollSpeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
	m_fMoveSpeed = (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" );

	if( m_pItems ) delete m_pItems;
	m_pItems = new Items();

	if( m_pPlane ) delete m_pPlane;
	m_pPlane = new Plane( m_pItems, this );
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
			case GS_PLAYON: 
			{
				InitNewGame();
				PlayOn();
				break;
			}
			default: break;
		}
	}
	Video::GetInstance()->Close();
}


/*********************************/
/**** PlayOn *********************/
/*********************************/

void Game::PlayOn()
{
	m_iFrameCnt = 0;

	while( m_eGameState == GS_PLAYON )
	{
		int A = SDL_GetTicks();
		HandleEventsPlayOn();
		if( !m_bPaused ) 
		{
			UpdateGameState();
		}
		DrawPlayOn();
		int iTime = SDL_GetTicks() - m_iSdlTicks;
		//if( iTime < 16 )
		//{
		//	SDL_Delay( 16 - iTime );
		//}
		m_iSdlTicks = SDL_GetTicks();
		m_iFrameCnt++;

		int B = SDL_GetTicks();
		iMs = B-A;
	}
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
				case SDLK_r:
					{
						m_eGameState = GS_INTRO;
						break;
					}
				case SDLK_g:
					{
						m_pPlane->GoCenter();
						break;
					}
				case SDLK_F12:
					{
						PrefsManager::GetInstance()->LoadPrefs("prefs.ini");
						m_fScrollSpeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
						m_bDebug = PrefsManager::GetInstance()->GetValue("DEBUG")==0?false:true;
						m_pPlane->SetMaxVel( (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" ) );
						m_fAcceleration = (float)PrefsManager::GetInstance()->GetValue( "ACCELERATION" );
						m_pPlane->ReloadConfig();
						m_pItems->ReloadConfig();
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

	if( m_fAcceleration > 0 )
	{
		m_fScrollSpeed += (float)dT/m_fAcceleration;
		m_fMoveSpeed += (float)dT/m_fAcceleration;
	}

	if( m_iEnding == 0)
	{
		if( m_bLeftDown )
			m_pPlane->GoLeft();
		else if( m_bRightDown )
			m_pPlane->GoRight();

		m_pItems->Generate( dT );
	}
	else if( m_iEnding == 1 )
	{
		m_pPlane->GoCenter();
	}
	else if( m_iEnding == 2 )
	{
		m_pItems->GenerateEnd();
	}

	m_pItems->SetScrollSpeed( m_fScrollSpeed );
	m_pItems->Update( dT );

	m_pPlane->SetMaxVel( m_fMoveSpeed );
	m_pPlane->Move( dT );
	m_pPlane->PickUpItems();

	m_pItems->ExpireItems();
}

void Game::DrawPlayOn()
{
	DrawBackground();
	m_pPlane->DrawPlane( m_pScreen );
	m_pItems->Draw( m_pScreen );

	if( m_bDebug )
	{
		if( m_eGameState == GS_PLAYON )
			DrawTime();
		DrawDebugInfos();
	}

	if (m_bPaused) DrawPaused();

	Video::GetInstance()->Flip();

	m_iFrameCnt++;
}

void Game::DrawBackground()
{
	m_iEnding = m_pBackground->Draw(m_pScreen, (int) (m_fActBackgoundPos + 0.5) );
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
		m_pDebugFont->DrawInt(m_pScreen, (m_pScreen->w / 2) - (m_iDebugFontSize * digitCnt) / 2, 5, timeToDraw, digitCnt, 0);
	}
}

void Game::DrawDebugInfos()
{
	static vector<int> oFrameTime;
	static int iPos = 0;
	string debuginfo =  "Scroll speed : " + asString( (int)m_fScrollSpeed );
	m_pDebugFont->DrawStr( m_pScreen, 10, 10, debuginfo );

	debuginfo =  "Move speed : " + asString( (int)m_fMoveSpeed );
	m_pDebugFont->DrawStr( m_pScreen, 10, 30, debuginfo );

	debuginfo =  "Score : " + asString( m_iScore );
	m_pDebugFont->DrawStr( m_pScreen, 10, 50, debuginfo );

	if( iMs > 0 )
	{
		debuginfo =  "Frame time : " + asString( iMs );
		m_pDebugFont->DrawStr( m_pScreen, 10, 70, debuginfo );

		debuginfo =  "FPS : " + asString( 1000 / iMs );
		m_pDebugFont->DrawStr( m_pScreen, 10, 90, debuginfo );
	}
}

void Game::DrawPaused()
{
	SDL_Rect r;
	r.x = m_pScreen->w/2 - m_pPauseSprite->w/2;
	r.y = m_pScreen->h/2 - m_pPauseSprite->h/2;
	r.w = m_pPauseSprite->w;
	r.h = m_pPauseSprite->h;
	Video::GetInstance()->DrawRect( m_iPauseSprite, 0, &r );
}

void Game::PickUpCoin()
{
	m_iScore++;
}

void Game::EnterCloud()
{
	m_iScore--;
	if( m_iScore < 0 )
		m_iScore = 0;
}

void Game::EnterThunder()
{
	m_iScore-=2;
	if( m_iScore < 0 )
		m_iScore = 0;
}

void Game::LoadResources()
{
	TextureManager* pTextureManager = TextureManager::GetInstance();
	pTextureManager->LoadSurface( "../../resources/imgs/coins.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/coins2.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Orage_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_004.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_005.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_006.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_007.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_008.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_009.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_010.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_011.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Orage_012.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_004.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_005.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_006.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_007.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_008.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_009.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_010.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_011.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Foudre_012.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_004.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_005.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_006.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_007.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_008.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_009.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_010.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_011.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_012.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_013.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_014.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_015.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_016.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_017.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_018.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_019.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_020.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_021.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_022.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_023.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_024.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_025.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_026.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_027.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_028.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_029.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_030.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_031.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_032.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_033.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_034.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_035.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_036.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_037.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_038.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_039.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_040.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_041.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_042.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_043.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_044.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_045.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_046.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_047.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_048.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_049.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_050.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_051.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_052.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_053.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_054.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_055.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_056.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_057.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_058.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_059.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_060.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_061.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_062.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_063.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_064.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_065.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_066.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_067.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_068.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_069.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_070.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_071.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_072.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_073.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_074.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Avion_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_004.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_005.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_006.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_007.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_008.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_009.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_010.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_011.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_004.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_004.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_005.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_006.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_007.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_008.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_009.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_010.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_004.png" );

	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_000.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_001.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_002.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_003.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_004.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_005.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_006.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_007.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_008.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_009.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_010.png" );
}

