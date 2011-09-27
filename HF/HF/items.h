#ifndef ITEMS_H
#define ITEMS_H

#include "SDL.h"
#include <vector>

class Item;
class Vector2D;

class Items
{
	vector<Item *> items;

	int m_iTimeNextItemAppear;
	int m_iItemAppearDelay;
	int m_iItemAppearRandomDelay;
	int m_iItemLifeTime;
	int m_iScreenWidth;
	int m_iScreenHeight;
	int m_iAnimDelay;
	float m_fScrollSpeed;
	float m_bEnd;

public:
	Items();
	~Items();

	void ReloadConfig();
	void AddItem(Item *item);
	void GenerateItemNow( Vector2D pos, Vector2D vel );
	void Generate( int dT );
	void GenerateEnd();
	void ExpireItems();
	void Update( int dT );
	void Draw(SDL_Surface *screen);
	void DeleteAllItems();
	void SetScrollSpeed( float fScrollSpeed ){ m_fScrollSpeed = fScrollSpeed; }

	inline unsigned int GetNrItems() { return items.size(); }
	Item *GetItem(unsigned int idx);
};


#endif
