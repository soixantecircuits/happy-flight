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
	ITEM_THUNDER,
	ITEM_END
};

class Item
{
	int m_iSprite;
	SDL_Surface *m_pSprite;
	std::vector< int > m_oSprites;
	int m_iCurrentSprite;

	Vector2D m_vPos;
	Vector2D m_vVel;
	Vector2D m_vSize;
	ItemTypes m_eItemType;

	int m_iTimeLived;
	int m_iItemLifeTime;
	int m_iScreenWidth;

	bool m_bPickedUp;

public:
	Item( const Vector2D &position, const Vector2D &velocity, ItemTypes itemType,  int iLifeTime, int iScreenWidth );
	~Item();

	void Update( int dT );
	void UpdateAnim();

	void DeleteItem();

	void Draw(SDL_Surface *screen);
	void PickedUp();
	bool IsPickedUp(){ return m_bPickedUp; }

	inline bool IsExpired() { return ( m_eItemType != ITEM_END && m_iTimeLived >= m_iItemLifeTime); }
	inline Vector2D GetPos() { return m_vPos; }
	inline Vector2D GetVel() { return m_vVel; }
	inline Vector2D GetSize() { return m_vSize; }
	inline ItemTypes GetType() { return m_eItemType; }
};

#endif
