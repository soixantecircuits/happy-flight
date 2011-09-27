#include <iostream>
#include "Score.h"
#include "TextureManager.h"
#include "video.h"

Score::Score()
{
	m_iScore = 0;

	TextureManager* pTextureManager = TextureManager::GetInstance();

	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_00.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_01.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_02.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_03.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_04.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_05.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_06.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_07.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_08.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Chiffre_09.png" ) );
	m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Score.png" ) );

	m_pNumberSprite = pTextureManager->GetTextureById( m_oSprites.front() );
	m_pScoreSprite = pTextureManager->GetTextureById( m_oSprites.back() );
}

Score::~Score()
{

}

void Score::SetScore( int iValue )
{
	m_iScore = iValue;
}

void Score::Draw(SDL_Surface *screen)
{
	SDL_Rect r;
	r.x = 10;
	r.y = 10;
	r.w = m_pScoreSprite->w;
	r.h = m_pScoreSprite->h;
	Video::GetInstance()->DrawRect( m_oSprites.back(), 0, &r );

	r.x = r.x + r.w + 10;
	r.w = m_pNumberSprite->w;
	r.h = m_pNumberSprite->h;
	Video::GetInstance()->DrawRect( m_oSprites[ (m_iScore / 10) % 10 ], 0, &r );

	r.x = r.x + r.w;
	Video::GetInstance()->DrawRect( m_oSprites[ m_iScore % 10 ], 0, &r );
}

