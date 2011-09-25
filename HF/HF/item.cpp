#include <iostream>
#include "item.h"
#include "surfaceDB.h"

Item::Item( const Vector2D &position, const Vector2D &velocity, ItemTypes itemType, int iLifeTime )
{
	m_iItemLifeTime = iLifeTime;
	m_eItemType = itemType;

	m_vPos = position;
	m_vVel = velocity;
	m_iTimeLived = 0;

	switch (itemType)
	{
		case ITEM_COIN:
		{
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/COINS/coins.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/COINS/coins2.png" ) );
			break;
		}
		case ITEM_CLOUD:
		{
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_000.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_001.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_002.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_003.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_004.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_005.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_006.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_007.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_008.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_009.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_010.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_011.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/ORAGE/Orage_012.png" ) );
			break;
		}
		case ITEM_THUNDER:
		{
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_000.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_001.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_002.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_003.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_004.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_005.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_006.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_007.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_008.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_009.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_010.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_011.png" ) );
			m_oSprites.push_back( surfaceDB.LoadSurface( "../../resources/imgs/FOUDRE/Foudre_012.png" ) );
			break;
		}
	}
	m_iCurrentSprite = 0;
	m_pSprite = m_oSprites.front();
}

Item::~Item()
{

}

void Item::Update( int dT )
{
	m_vPos += m_vVel * (float)dT / 1000.0;
	m_iTimeLived += dT;
}

void Item::UpdateAnim()
{
	if( m_eItemType != ITEM_COIN )
	{
		m_iCurrentSprite++;
		if( m_iCurrentSprite == m_oSprites.size() )
			m_iCurrentSprite = 0;
		m_pSprite = m_oSprites[m_iCurrentSprite];
	}
}

void Item::DeleteItem()
{
	m_iTimeLived = m_iItemLifeTime;
}

void Item::Draw(SDL_Surface *screen)
{
	SDL_Rect r;
	r.x = (Sint16)lroundf(m_vPos.getX()) - m_pSprite->w / 2;
	r.y = (Sint16)lroundf(m_vPos.getY()) - m_pSprite->h / 2;
	r.w = m_pSprite->w;
	r.h = m_pSprite->h;
	SDL_BlitSurface( m_pSprite, 0, screen, &r );
}

