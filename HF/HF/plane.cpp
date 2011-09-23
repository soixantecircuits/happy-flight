using namespace std;

#include "plane.h"
#include "surfaceDB.h"
#include "PrefsManager.h"

int screenWidth, screenHeight, animDelay;

float lroundf( float value )
{
	return (float)floor( value + 0.5 );
}

Plane::Plane()
{
	maxVel = (float)PrefsManager::GetInstance()->GetValue( "MOVE_SPEED" );
	screenWidth = PrefsManager::GetInstance()->GetValue( "SCREEN_WIDTH" );
	screenHeight = PrefsManager::GetInstance()->GetValue( "SCREEN_HEIGHT" );
	animDelay = PrefsManager::GetInstance()->GetValue( "ANIM_DELAY" );
	spriteRacerBase = surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_000.png" );

	m_pCurrentAnim = &m_oSpritesUp;

	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_000.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_001.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_002.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_003.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_004.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_005.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_006.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_007.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_008.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_009.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_010.png" ) );
	m_oSpritesUp.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\UP\\Avion_011.png" ) );

	m_oSpritesTurnLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_LEFT\\Avion_Turn_L_000.png" ) );
	m_oSpritesTurnLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_LEFT\\Avion_Turn_L_001.png" ) );
	m_oSpritesTurnLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_LEFT\\Avion_Turn_L_002.png" ) );
	m_oSpritesTurnLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_LEFT\\Avion_Turn_L_003.png" ) );
	m_oSpritesTurnLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_LEFT\\Avion_Turn_L_004.png" ) );

	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_000.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_001.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_002.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_003.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_004.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_005.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_006.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_007.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_008.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_009.png" ) );
	m_oSpritesLeft.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_LEFT\\Avion_Left_010.png" ) );

	m_oSpritesTurnRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_RIGHT\\Avion_Turn_R_000.png" ) );
	m_oSpritesTurnRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_RIGHT\\Avion_Turn_R_001.png" ) );
	m_oSpritesTurnRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_RIGHT\\Avion_Turn_R_002.png" ) );
	m_oSpritesTurnRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_RIGHT\\Avion_Turn_R_003.png" ) );
	m_oSpritesTurnRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\TURN_RIGHT\\Avion_Turn_R_004.png" ) );

	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_000.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_001.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_002.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_003.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_004.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_005.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_006.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_007.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_008.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_009.png" ) );
	m_oSpritesRight.push_back( surfaceDB.loadSurface( "..\\..\\resources\\imgs\\AVION\\VOLE_RIGHT\\Avion_Right_010.png" ) );

	m_iCurrentSprite = 0;

	drawRectBase.w = spriteRacerBase->w;
	drawRectBase.h = spriteRacerBase->h;
	pos = Vector2D( (float)screenWidth/2, (float)screenHeight-160 );

	realSize = Vector2D( 420, 300 ); //l'avion est moins grand que le png
	vel = Vector2D( 0,0 );
	left = false;
	right = false;

	timeLastMove = SDL_GetTicks();
}


Plane::~Plane()
{

}

Vector2D Plane::move( int dT )
{
	static int elapsedTime = 0;
	static bool oldleft, oldright;
	elapsedTime += dT;
	if( elapsedTime > animDelay || oldleft != left || oldright != right )
	{
		elapsedTime = 0;
		UpdateAnim();
	}

	oldright = right;
	oldleft = left;

	Vector2D oldPos = pos;

	if( left )
	{
		pos += Vector2D( -maxVel, 0 ) * (float)dT / 1000.0;
	} 
	if( right )
	{
		pos += Vector2D( maxVel, 0 ) * (float)dT / 1000.0;
	}

	clipWorld();

	return oldPos;
}

void Plane::UpdateAnim()
{
		if( m_pCurrentAnim == &m_oSpritesTurnLeft )
	{
		if( left )
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
				if( right )
					m_pCurrentAnim = &m_oSpritesTurnRight;
				else
					m_pCurrentAnim = &m_oSpritesUp;
			}
		}
	}
	else if( m_pCurrentAnim == &m_oSpritesTurnRight )
	{
		if( right )
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
				if( left )
					m_pCurrentAnim = &m_oSpritesTurnLeft;
				else
					m_pCurrentAnim = &m_oSpritesUp;
			}
		}
	}
	else if( m_pCurrentAnim == &m_oSpritesLeft )
	{
		if( left )
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
		if( right )
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
		if( left )
		{
			m_pCurrentAnim = &m_oSpritesTurnLeft;
			m_iCurrentSprite = 0;
		}
		else if( right )
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

	spriteRacerBase = (*m_pCurrentAnim)[ m_iCurrentSprite ];
}

void Plane::clipWorld()
{
	int left = (int)( lroundf(pos.getX()) - (realSize.getX() / 2) );
	int right = (int)( lroundf(pos.getX()) + (realSize.getX() / 2) );
	if ( left <= 1 )
	{
		pos.setX( (float)( 1 + realSize.getX() / 2 ) );
	}
	else if ( right >= screenWidth - 1 )
	{
		pos.setX( (float)( screenWidth - 2 - realSize.getX() / 2 ) );
	}
}

void Plane::drawStats( SDL_Surface *screen )
{

}


void Plane::drawShadow( SDL_Surface *screen )
{
	//SDL_Rect destR;
	//destR.x = lroundf(pos.getX()) - (spriteShadow->w / 2) - 10;
	//destR.y = lroundf(pos.getY()) - (spriteShadow->h / 2) + 10;
	//destR.w = spriteShadow->w;
	//destR.h = spriteShadow->h;
	//SDL_BlitSurface( spriteShadow, 0, screen, &destR );
}


void Plane::drawPlane( SDL_Surface *screen )
{
	SDL_Rect srcR;
	SDL_Rect destR;

	destR.x = Sint16( lroundf(pos.getX()) - (spriteRacerBase->w / 2) );
	destR.y = Sint16( lroundf(pos.getY()) - (spriteRacerBase->h / 2) );
	destR.w = spriteRacerBase->w;
	destR.h = spriteRacerBase->h;

	srcR.x = 0;
	srcR.y = 0;
	srcR.w = spriteRacerBase->w;
	srcR.h = spriteRacerBase->h;

	SDL_BlitSurface( spriteRacerBase, &srcR, screen, &destR );
}

void Plane::setMaxVel( float newVel )
{
	maxVel = newVel;
}

void Plane::setPos( const Vector2D &newPos )
{
	pos = newPos;
}

void Plane::pickUpItems()
{
	//for ( unsigned int i = 0; i < items->getNrItems(); i++ )
	//{
	//	Item *item = items->getItem(i);
	//	if ( boundingBox->overlaps( item->getBoundingBox() ) )
	//	{
	//		item->pickedUp();
	//	}
	//}
}

void Plane::handlePlayerEvent( PlayerEvent pEvent, bool keyDown )
{
	switch (pEvent)
	{
		case PE_LEFT: if ( keyDown ) left = true; else left = false; break;
		case PE_RIGHT: if ( keyDown ) right = true; else right = false; break;
		default: break;
	}
}

