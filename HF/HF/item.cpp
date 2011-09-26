#include <iostream>
#include "item.h"
#include "surfaceDB.h"
#include "video.h"

Item::Item( const Vector2D &position, const Vector2D &velocity, ItemTypes itemType, int iLifeTime, int iScreenWidth )
{
	m_iItemLifeTime = iLifeTime;
	m_eItemType = itemType;
	m_iScreenWidth = iScreenWidth;

	m_bPickedUp = false;
	m_vPos = position;
	m_vVel = velocity;
	m_iTimeLived = 0;

	switch (itemType)
	{
		case ITEM_COIN:
		{
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/coins.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/coins2.png" ) );
			break;
		}
		case ITEM_CLOUD:
		{
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_000.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_001.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_002.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_003.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_004.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_005.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_006.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_007.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_008.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_009.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_010.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_011.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Orage_012.png" ) );
			break;
		}
		case ITEM_THUNDER:
		{
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_000.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_001.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_002.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_003.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_004.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_005.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_006.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_007.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_008.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_009.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_010.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_011.png" ) );
			m_oSprites.push_back( TextureManager::GetInstance()->LoadSurface( "../../resources/imgs/Foudre_012.png" ) );
			break;
		}
	}
	m_iCurrentSprite = 0;
	m_iSprite = m_oSprites.front();
	m_pSprite = TextureManager::GetInstance()->GetTextureById( m_oSprites.front() );

	m_vSize = Vector2D( (float)m_pSprite->w, (float)m_pSprite->h );
}

Item::~Item()
{

}

void Item::Update( int dT )
{
	m_vPos += m_vVel * (float)dT / 1000.0;
	if( m_vPos.getX() < 100 || m_vPos.getX() > ( m_iScreenWidth - 100) )
		m_vVel.setX( -m_vVel.getX() );
	m_iTimeLived += dT;
}

void Item::UpdateAnim()
{
	if( m_eItemType != ITEM_COIN )
	{
		m_iCurrentSprite++;
		if( m_iCurrentSprite == m_oSprites.size() )
			m_iCurrentSprite = 0;
		m_iSprite = m_oSprites[m_iCurrentSprite];
	}
}

void Item::DeleteItem()
{
	m_iTimeLived = m_iItemLifeTime;
}

void Item::Draw(SDL_Surface *screen)
{
	SDL_Rect r;
	r.x = (Sint16)ROUND(m_vPos.getX()) - m_pSprite->w / 2;
	r.y = (Sint16)ROUND(m_vPos.getY()) - m_pSprite->h / 2;
	r.w = m_pSprite->w;
	r.h = m_pSprite->h;
	Video::GetInstance()->DrawRect( m_iSprite, 0, &r );
}

void Item::PickedUp()
{
	if( m_eItemType == ITEM_COIN )
		m_iTimeLived = m_iItemLifeTime;
	m_bPickedUp = true;
}

