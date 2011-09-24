#include "item.h"
#include "items.h"
#include "surfaceDB.h"
#include "geometry.h"
#include "PrefsManager.h"

const int NR_ITEM_TYPES = 3;
int ITEM_APPEAR_CHANCES[] = { 70, 20, 10 };

int GetRandValue( const int *choicesWeights, int nrChoices )
{
	int sum = 0;
	for( int i = 0; i < nrChoices; i++ ) sum += choicesWeights[ i ];

	int val = rand() % sum;

	int idx = 0;
	int tmpSum = 0;
	while ( idx < nrChoices )
	{
		tmpSum += choicesWeights[ idx ];
		if ( val < tmpSum )
		{
			return idx;
		}
		idx++;
	}
}

Items::Items()
{
	m_iItemAppearDelay = PrefsManager::GetInstance()->GetValue( "ITEM_APPEAR_DELAY" );
	m_iItemAppearRandomDelay = PrefsManager::GetInstance()->GetValue( "ITEM_APPEAR_RAND_DELAY" );
	m_iScreenWidth = PrefsManager::GetInstance()->GetValue( "SCREEN_WIDTH" );
	m_iTimeNextItemAppear = m_iItemAppearDelay + (rand() % m_iItemAppearRandomDelay);
}

Items::~Items()
{
	vector<Item *>::iterator i;
	for(i = items.begin(); i != items.end(); ++i)
	{
		delete *i;
	}
}

void Items::AddItem(Item *item)
{
	if(item)
	{
		items.push_back(item);
	}
}

void Items::ExpireItems()
{
	unsigned int i = 0;
	while ( i < items.size() )
	{
		if ( items[i]->IsExpired() )
		{
			delete items[i];
			items.erase(items.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void Items::Update( int dT )
{
	vector<Item *>::iterator i;
	for (i = items.begin(); i != items.end(); ++i)
	{
		(*i)->Update( dT );
	}
}

void Items::Draw(SDL_Surface *screen)
{
	vector<Item *>::iterator i;
	for (i = items.begin(); i != items.end(); ++i)
	{
		(*i)->Draw(screen);
	}
}

Item *Items::GetItem(unsigned int idx)
{
	return items[idx];
}


void Items::GenerateItemNow( Vector2D pos, Vector2D vel )
{
	if( pos.getX() < 10 ) pos.setX( 10.f );
	if( pos.getX() > m_iScreenWidth-10 ) pos.setX( m_iScreenWidth-10.f );
	if( pos.getY() < 100 && vel.getY() < 5 ) vel.setY( 5 );

	int itemType = GetRandValue( ITEM_APPEAR_CHANCES, NR_ITEM_TYPES );
	Item *item = new Item( pos, vel, (ItemTypes)itemType );
	AddItem( item );
}

void Items::Generate( int dT )
{
	m_iTimeNextItemAppear -= dT;

	if ( m_iTimeNextItemAppear < 0 )
	{
		m_iTimeNextItemAppear = m_iItemAppearDelay + (rand() % m_iItemAppearRandomDelay);
		GenerateItemNow( Vector2D( 150.f + (rand() % 340), -20.f ),
		Vector2D( (rand() % (int)lroundf(m_fScrollSpeed)) - m_fScrollSpeed / 2, m_fScrollSpeed + (rand() % (int)lroundf(m_fScrollSpeed)) ) );
	}
}

void Items::DeleteAllItems()
{
	vector<Item *>::iterator i;
	for (i = items.begin(); i != items.end(); ++i)
	{
		(*i)->DeleteItem();
	}
	ExpireItems();
}
