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
#include "Score.h"
#include "ofxOsc.h"

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
	m_bOSCLeft = false;
	m_bOSCRight = false;

	m_iScore = 0;

	// needed for calculating fps
	m_iFrameCnt = 0;

	m_eGameState = GS_INTRO;
	m_bPaused = true;
	m_iSdlTicks = SDL_GetTicks();

	m_iPauseSprite = TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/paused.png" );
	m_pPauseSprite = TextureManager::GetInstance()->GetTextureById( m_iPauseSprite );
	m_pDebugFont = new Font( "../../resources/imgs/font-20red.png" );
	m_iDebugFontSize = m_pDebugFont->GetCharW();

	LoadResources();

	m_bScrolling = true;
	m_pBackground = new Background();
	m_pBackground->GenerateBackground( PrefsManager::GetInstance()->GetValue("BKG_LENGTH") );

	m_pItems = NULL;
	m_pPlane = NULL;
	m_pScore = NULL;
	
	cout << "listening for osc messages on port " << PORT << "\n";
	m_pReceiver = new ofxOscReceiver();
	m_pReceiver->setup( PORT );
}

Game::~Game()
{
	if( m_pPlane ) delete m_pPlane;
	if( m_pItems ) delete m_pItems;
	if( m_pScore ) delete m_pScore;

	TextureManager::GetInstance()->Delete();
	Video::GetInstance()->Delete();
}

void Game::InitNewGame()
{
	m_bEnd = false;
	m_iScore = 0;
	m_fScrollSpeed = (float)PrefsManager::GetInstance()->GetValue( "SCROLL_SPEED" );
	m_fMoveSpeed = (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" );
	m_pBackground->SetState( E_START );

	if( m_pScore ) delete m_pScore;
	m_pScore = new Score();

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
		HandleOSCEventsPlayOn();
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

void Game::HandleOSCEventsPlayOn()
{
	// check for waiting messages
		while( m_pReceiver->hasWaitingMessages() )
		{
			// get the next message
			ofxOscMessage m;
			m_pReceiver->getNextMessage( &m );

			// check for mouse moved message
			if ( m.getAddress() == "/keyboard/left" )
			{
				if (m.getArgAsInt32( 0 ) == 1){
					m_bOSCLeft = true;
				} else m_bOSCLeft = false;
			}
			// check for mouse button message
			else if ( m.getAddress() == "/keyboard/right" )
			{
				if (m.getArgAsInt32( 0 ) == 1){
					m_bOSCRight = true;
				} else m_bOSCRight = false;
			}
			else
			{
				// unrecognized message: display on the bottom of the screen
				string msg_string;
				msg_string = m.getAddress();
				msg_string += ": ";
				for ( int i=0; i<m.getNumArgs(); i++ )
				{
					// get the argument type
					msg_string += m.getArgTypeName( i );
					msg_string += ":";
					// display the argument - make sure we get the right type
					if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
						msg_string += m.getArgAsInt32( i ) ;
					else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
						msg_string += asString( m.getArgAsFloat( i ) );
					else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
						msg_string += m.getArgAsString( i );
					else
						msg_string += "unknown";
				}
				// add to the list of strings to display
				cout << msg_string << endl;
			}

		}
	
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

	if( m_pBackground->GetState() == E_TAKEOFF )
	{
		m_pPlane->SetState( E_PLANE_TAKE_OFF );
	}
	else if( m_pBackground->GetState() == E_FLYING || m_pBackground->GetState() == E_APPROACH )
	{
		m_pPlane->SetState( E_PLANE_FLYING );
		if( m_bLeftDown || m_bOSCLeft )
			m_pPlane->GoLeft();
		else if( m_bRightDown || m_bOSCRight )
			m_pPlane->GoRight();

		if( m_pBackground->GetState() == E_FLYING )
			m_pItems->Generate( dT );
		else
			m_pItems->DeleteAllItems();
	}
	else if( m_pBackground->GetState() == E_LANDING )
	{
		m_pPlane->SetState( E_PLANE_LANDING );
	}
	else if( m_pBackground->GetState() == E_STOP )
	{
		if( !m_bEnd )
		{
			m_pItems->GenerateEnd();
			m_fScrollSpeed = 0;
		}
		m_bEnd = true;
	}

	m_pItems->SetScrollSpeed( m_fScrollSpeed );
	m_pItems->Update( dT );

	m_pPlane->SetMaxVel( m_fMoveSpeed );
	m_pPlane->Move( dT );
	m_pPlane->PickUpItems();

	m_pItems->ExpireItems();

	m_pScore->SetScore( m_iScore );
}

void Game::DrawPlayOn()
{
	DrawBackground();
	m_pPlane->DrawPlane( m_pScreen );
	m_pItems->Draw( m_pScreen );
	m_pScore->Draw( m_pScreen );

	if( m_bDebug )
	{
		DrawDebugInfos();
	}

	if (m_bPaused) DrawPaused();

	Video::GetInstance()->Flip();

	m_iFrameCnt++;
}

void Game::DrawBackground()
{
	m_pBackground->Draw(m_pScreen, (int) (m_fActBackgoundPos + 0.5) );
}

void Game::DrawDebugInfos()
{
	static vector<int> oFrameTime;
	static int iPos = 0;
	string debuginfo =  "Scroll speed : " + asString( (int)m_fScrollSpeed );
	m_pDebugFont->DrawStr( m_pScreen, 10, 110, debuginfo );

	debuginfo =  "Move speed : " + asString( (int)m_fMoveSpeed );
	m_pDebugFont->DrawStr( m_pScreen, 10, 130, debuginfo );

	debuginfo =  "Score : " + asString( m_iScore );
	m_pDebugFont->DrawStr( m_pScreen, 10, 150, debuginfo );

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

}

void Game::EnterThunder()
{
	m_iScore--;
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

	pTextureManager->LoadSurface( "../../resources/imgs/Score.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_00.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_01.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_02.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_03.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_04.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_05.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_06.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_07.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_08.png" );
	pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_09.png" );
}

