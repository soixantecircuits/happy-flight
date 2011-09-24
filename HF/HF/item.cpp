#include <iostream>
#include "item.h"
#include "surfaceDB.h"

Item::Item(const Vector2D &position, const Vector2D &velocity, ItemTypes itemType) {

	this->m_eItemType = itemType;

	m_vPos = position;
	m_vVel = velocity;
	m_iTimeLived = 0;

	switch (itemType)
	{
		case ITEM_COIN:
		{
			m_pSprite = surfaceDB.LoadSurface( "..\\..\\resources\\img\\COINS\\coins.png" );
			break;
		}
		case ITEM_CLOUD:
		{
			m_pSprite = surfaceDB.LoadSurface( "..\\..\\resources\\img\\ORAGE\\Orage_000.png" );
			break;
		}
		case ITEM_THUNDER:
		{
			m_pSprite = surfaceDB.LoadSurface( "..\\..\\resources\\img\\FOUDRE\\Foudre_000.png" );
			break;
		}
	}
}

Item::~Item()
{

}

void Item::Update( int dT )
{
	m_vPos += m_vVel * (float)dT / 1000.0;
	m_iTimeLived += dT;
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

