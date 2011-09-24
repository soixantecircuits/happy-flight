#ifndef ITEM_H
#define ITEM_H

#include "SDL.h"
#include "geometry.h"
#include <string>
#include <vector>

enum ItemTypes
{ 
	ITEM_COIN=0,
	ITEM_CLOUD,
	ITEM_THUNDER
};

class Item
{
	SDL_Surface *m_pSprite;
	std::vector< SDL_Surface* > m_oSprites;
	int m_iCurrentSprite;

	Vector2D m_vPos;
	Vector2D m_vVel;
	ItemTypes m_eItemType;

	int m_iTimeLived;
	int m_iItemLifeTime;

public:
	Item( const Vector2D &position, const Vector2D &velocity, ItemTypes itemType,  int iLifeTime );
	~Item();

	void Update( int dT );
	void UpdateAnim();

	void DeleteItem();

	void Draw(SDL_Surface *screen);

	inline bool IsExpired() { return (m_iTimeLived >= m_iItemLifeTime); }
	inline Vector2D GetPos() { return m_vPos; }
	inline Vector2D GetVel() { return m_vVel; }
	inline void PickedUp() { m_iTimeLived = m_iItemLifeTime; }
	inline ItemTypes GetType() { return m_eItemType; }
};

#endif
