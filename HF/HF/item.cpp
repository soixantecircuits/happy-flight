#include <iostream>
#include "item.h"
#include "TextureManager.h"
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

	TextureManager* pTextureManager = TextureManager::GetInstance();

	switch (itemType)
	{
		case ITEM_COIN:
		{
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/coins.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/coins2.png" ) );
			break;
		}
		case ITEM_CLOUD:
		{
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_000.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_001.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_002.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_003.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_004.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_005.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_006.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_007.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_008.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_009.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_010.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_011.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Orage_012.png" ) );
			break;
		}
		case ITEM_THUNDER:
		{
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_000.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_001.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_002.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_003.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_004.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_005.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_006.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_007.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_008.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_009.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_010.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_011.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/Foudre_012.png" ) );
			break;
		}
		case ITEM_END:
		{
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_000.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_001.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_002.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_003.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_004.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_005.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_006.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_007.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_008.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_009.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_010.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_011.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_012.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_013.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_014.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_015.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_016.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_017.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_018.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_019.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_020.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_021.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_022.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_023.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_024.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_025.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_026.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_027.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_028.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_029.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_030.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_031.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_032.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_033.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_034.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_035.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_036.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_037.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_038.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_039.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_040.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_041.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_042.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_043.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_044.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_045.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_046.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_047.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_048.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_049.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_050.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_051.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_052.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_053.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_054.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_055.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_056.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_057.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_058.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_059.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_060.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_061.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_062.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_063.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_064.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_065.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_066.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_067.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_068.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_069.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_070.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_071.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_072.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_073.png" ) );
			m_oSprites.push_back( pTextureManager->LoadSurface( "../../resources/imgs/EndofGame_074.png" ) );
			break;
		}
	}
	m_iCurrentSprite = 0;
	m_iSprite = m_oSprites.front();
	m_pSprite = pTextureManager->GetTextureById( m_oSprites.front() );

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

