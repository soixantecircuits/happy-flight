using namespace std;

#include "plane.h"
#include "TextureManager.h"
#include "PrefsManager.h"
#include "game.h"
#include "items.h"
#include "item.h"
#include "video.h"

Plane::Plane( Items* pItems, Game* pGame )
: m_pItems( pItems )
, m_pGame( pGame )
{
	m_eState = E_PLANE_START;
	m_iScreenWidth = PrefsManager::GetInstance()->GetValue( "GAME_WIDTH" );
	m_iScreenHeight = PrefsManager::GetInstance()->GetValue( "GAME_HEIGHT" );
	m_iAnimDelay = PrefsManager::GetInstance()->GetValue( "ANIM_DELAY" );
	m_iPickDistance = PrefsManager::GetInstance()->GetValue( "PICK_DISTANCE" );
	m_iPlaneDist = PrefsManager::GetInstance()->GetValue( "PLANE_DIST" );
	int iPlaneWidth = PrefsManager::GetInstance()->GetValue( "PLANE_WIDTH" );
	int iPlaneHeight = PrefsManager::GetInstance()->GetValue( "PLANE_HEIGHT" );

	TextureManager* pTextureManager = TextureManager::GetInstance();

	m_iSpriteBase = pTextureManager->LoadSurface( "../../resources/imgs/Avion_000.png" );
	m_pCurrentAnim = &m_oSpritesUp;

	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_000.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_001.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_002.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_003.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_004.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_005.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_006.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_007.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_008.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_009.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_010.png" ) );
	m_oSpritesUp.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_011.png" ) );

	m_oSpritesTurnLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_000.png" ) );
	m_oSpritesTurnLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_001.png" ) );
	m_oSpritesTurnLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_002.png" ) );
	m_oSpritesTurnLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_003.png" ) );
	m_oSpritesTurnLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_L_004.png" ) );

	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_000.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_001.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_002.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_003.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_004.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_005.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_006.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_007.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_008.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_009.png" ) );
	m_oSpritesLeft.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Left_010.png" ) );

	m_oSpritesTurnRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_000.png" ) );
	m_oSpritesTurnRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_001.png" ) );
	m_oSpritesTurnRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_002.png" ) );
	m_oSpritesTurnRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_003.png" ) );
	m_oSpritesTurnRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Turn_R_004.png" ) );

	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_000.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_001.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_002.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_003.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_004.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_005.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_006.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_007.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_008.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_009.png" ) );
	m_oSpritesRight.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Avion_Right_010.png" ) );

	m_iCurrentSprite = 0;

	m_pSpriteBase = pTextureManager->GetTextureById( m_iSpriteBase );

	m_oDrawRectBase.w = m_pSpriteBase->w;
	m_oDrawRectBase.h = m_pSpriteBase->h;
	m_vPos = Vector2D( (float)m_iScreenWidth/2, (float)m_iScreenHeight-m_iPlaneDist );

	m_vRealSize = Vector2D( (float)iPlaneWidth, (float)iPlaneHeight ); //l'avion est moins grand que le png
	m_vRealDisplaySize = Vector2D( (float)m_pSpriteBase->w, (float)m_pSpriteBase->h );
	m_vDisplaySize = m_vRealDisplaySize/2;
	m_vVel = Vector2D( 0,0 );
	m_bLeft = false;
	m_bRight = false;

	m_iTimeLastMove = SDL_GetTicks();
}


Plane::~Plane()
{

}

void Plane::ReloadConfig()
{
	m_iAnimDelay = PrefsManager::GetInstance()->GetValue( "ANIM_DELAY" );
	m_iPlaneDist = PrefsManager::GetInstance()->GetValue( "PLANE_DIST" );
	int iPlaneWidth = PrefsManager::GetInstance()->GetValue( "PLANE_WIDTH" );
	int iPlaneHeight = PrefsManager::GetInstance()->GetValue( "PLANE_HEIGHT" );

	m_vPos = Vector2D( (float)m_iScreenWidth/2, (float)m_iScreenHeight-m_iPlaneDist );
	m_vRealSize = Vector2D( (float)iPlaneWidth, (float)iPlaneHeight ); //l'avion est moins grand que le png
}

Vector2D Plane::Move( int dT )
{
	static int elapsedTime = 0;
	static bool oldleft, oldright;

	if( m_eState == E_PLANE_LANDING )
	{
		if( m_vPos.getX() > (float)m_iScreenWidth/2 )
			m_bLeft = true;
		else if( m_vPos.getX() < (float)m_iScreenWidth/2 )
			m_bRight = true;
		if( m_vPos.getX() == (float)m_iScreenWidth/2 )
		{
			m_vDisplaySize = m_vDisplaySize / 1.01f;
			if( m_vDisplaySize.getX() < m_vRealDisplaySize.getX()/2 )
				m_vDisplaySize = m_vRealDisplaySize/2;
		}
	}
	else if( m_eState == E_PLANE_TAKE_OFF ||
		( m_eState == E_PLANE_FLYING && ( m_vDisplaySize.getX() < m_vRealDisplaySize.getX() ) ) )
	{
		m_vDisplaySize = m_vDisplaySize * 1.01f;
		if( m_vDisplaySize.getX() > m_vRealDisplaySize.getX() )
			m_vDisplaySize = m_vRealDisplaySize;
	}

	elapsedTime += dT;
	if( elapsedTime > m_iAnimDelay || oldleft != m_bLeft || oldright != m_bRight )
	{
		elapsedTime = 0;
		UpdateAnim();
	}

	oldright = m_bRight;
	oldleft = m_bLeft;

	Vector2D oldPos = m_vPos;

	if( m_bLeft )
		m_vPos += Vector2D( -m_fMaxVel, 0 ) * (float)dT / 1000.0;

	if( m_bRight )
		m_vPos += Vector2D( m_fMaxVel, 0 ) * (float)dT / 1000.0;

	if( m_eState == E_PLANE_LANDING && ( (oldPos.getX() > (float)m_iScreenWidth/2) != (m_vPos.getX() > (float)m_iScreenWidth/2) ) )
		m_vPos.setX( (float)m_iScreenWidth/2 );

	ClipWorld();
	m_bLeft = false;
	m_bRight = false;

	return oldPos;
}

void Plane::UpdateAnim()
{
	if( m_pCurrentAnim == &m_oSpritesTurnLeft )
	{
		if( m_bLeft )
		{
			m_iCurrentSprite++;
			if( m_iCurrentSprite == m_oSpritesTurnLeft.size() )
			{
				m_iCurrentSprite = 0;
				m_pCurrentAnim = &m_oSpritesLeft;
			}
		}
		else
		{
			m_iCurrentSprite--;
			if( m_iCurrentSprite < 0 )
			{
				m_iCurrentSprite = 0;
				if( m_bRight )
					m_pCurrentAnim = &m_oSpritesTurnRight;
				else
					m_pCurrentAnim = &m_oSpritesUp;
			}
		}
	}
	else if( m_pCurrentAnim == &m_oSpritesTurnRight )
	{
		if( m_bRight )
		{
			m_iCurrentSprite++;
			if( m_iCurrentSprite == m_oSpritesTurnRight.size() )
			{
				m_iCurrentSprite = 0;
				m_pCurrentAnim = &m_oSpritesRight;
			}
		}
		else
		{
			m_iCurrentSprite--;
			if( m_iCurrentSprite < 0 )
			{
				m_iCurrentSprite = 0;
				if( m_bLeft )
					m_pCurrentAnim = &m_oSpritesTurnLeft;
				else
					m_pCurrentAnim = &m_oSpritesUp;
			}
		}
	}
	else if( m_pCurrentAnim == &m_oSpritesLeft )
	{
		if( m_bLeft )
		{
			m_iCurrentSprite++;
			if( m_iCurrentSprite == m_oSpritesLeft.size() )
				m_iCurrentSprite = 0;
		}
		else
		{
			m_pCurrentAnim = &m_oSpritesTurnLeft;
			m_iCurrentSprite = m_pCurrentAnim->size()-1;
		}
	}
	else if( m_pCurrentAnim == &m_oSpritesRight )
	{
		if( m_bRight )
		{
			m_iCurrentSprite++;
			if( m_iCurrentSprite == m_oSpritesRight.size() )
				m_iCurrentSprite = 0;
		}
		else
		{
			m_pCurrentAnim = &m_oSpritesTurnRight;
			m_iCurrentSprite = m_pCurrentAnim->size()-1;
		}
	}
	else if( m_pCurrentAnim == &m_oSpritesUp )
	{
		if( m_bLeft )
		{
			m_pCurrentAnim = &m_oSpritesTurnLeft;
			m_iCurrentSprite = 0;
		}
		else if( m_bRight )
		{
			m_pCurrentAnim = &m_oSpritesTurnRight;
			m_iCurrentSprite = 0;
		}
		else
		{
			m_iCurrentSprite++;
			if( m_iCurrentSprite == m_oSpritesUp.size() )
				m_iCurrentSprite = 0;
		}
	}

	m_iSpriteBase = (*m_pCurrentAnim)[ m_iCurrentSprite ];
}

void Plane::ClipWorld()
{
	int left = (int)( ROUND(m_vPos.getX()) - (m_vRealSize.getX() / 2) );
	int right = (int)( ROUND(m_vPos.getX()) + (m_vRealSize.getX() / 2) );
	if ( left <= 1 )
	{
		m_vPos.setX( (float)( 1 + m_vRealSize.getX() / 2 ) );
	}
	else if ( right >= m_iScreenWidth - 1 )
	{
		m_vPos.setX( (float)( m_iScreenWidth - 2 - m_vRealSize.getX() / 2 ) );
	}
}

void Plane::DrawPlane( SDL_Surface *screen )
{
	SDL_Rect srcR;
	SDL_Rect destR;

#ifdef OPENGL
	destR.x = Sint16( ROUND(m_vPos.getX()) - (m_vDisplaySize.getX() / 2) );
	destR.y = Sint16( ROUND(m_vPos.getY()) - (m_vDisplaySize.getY() / 2) );
	destR.w = (int)m_vDisplaySize.getX();
	destR.h = (int)m_vDisplaySize.getY();
#else
	destR.x = Sint16( ROUND(m_vPos.getX()) - (m_pSpriteBase->w / 2) );
	destR.y = Sint16( ROUND(m_vPos.getY()) - (m_pSpriteBase->h / 2) );
	destR.w = m_pSpriteBase->w;
	destR.h = m_pSpriteBase->h;
#endif

	srcR.x = 0;
	srcR.y = 0;
	srcR.w = m_pSpriteBase->w;
	srcR.h = m_pSpriteBase->h;

	Video::GetInstance()->DrawRect( m_iSpriteBase, &srcR, &destR );
}

void Plane::SetMaxVel( float newVel )
{
	m_fMaxVel = newVel;
}

void Plane::SetPos( const Vector2D &newPos )
{
	m_vPos = newPos;
}

void Plane::PickUpItems()
{
	for ( unsigned int i = 0; i < m_pItems->GetNrItems(); i++ )
	{
		Item* pItem = m_pItems->GetItem(i);
		if( pItem->IsPickedUp() )
			continue;

		Vector2D oPos = pItem->GetPos();

		if( fabs( oPos.getX() - m_vPos.getX() ) < m_iPickDistance && fabs( oPos.getY() - m_vPos.getY() ) < m_iPickDistance )
		{
			pItem->PickedUp();
			if( pItem->GetType() == ITEM_COIN )
			{
				m_pGame->PickUpCoin();
			}
			else if( pItem->GetType() == ITEM_CLOUD )
			{
				m_pGame->EnterCloud();
			}
			else if( pItem->GetType() == ITEM_THUNDER )
			{
				m_pGame->EnterThunder();
			}
		}
	}
}


