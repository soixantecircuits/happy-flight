#include "background.h"
#include "surfaceDB.h"
#include "SDL.h"
#include <iostream>
#include "PrefsManager.h"

using namespace std;

Background::Background()
{
	m_iMinTileWidth   = 9999999;
	m_iMinTileHeight  = 9999999;
	m_iTilesPerLine   = 0;
	m_iTilesPerColumn = 0;
	m_iStep           = 0;
}


void Background::ClearTileList()
{
	m_oTileNames.clear();
	m_iTilesPerLine   = 0;
	m_iTilesPerColumn = 0;
}


void Background::AddTile( string tilename )
{
	m_oTileNames.push_back( tilename );
}


void Background::GenerateBackground( int length )
{
	int iWidth = PrefsManager::GetInstance()->GetValue("SCREEN_WIDTH");
	int iHeight = PrefsManager::GetInstance()->GetValue("SCREEN_HEIGHT");

	m_oTileSurfaces.clear();
	m_iMinTileWidth   = 9999999;
	m_iMinTileHeight  = 9999999;

	AddTile( "../../resources/imgs/MAP/Tile_Foret_01.png" );
	AddTile( "../../resources/imgs/MAP/Tile_Foret_02.png" );
	AddTile( "../../resources/imgs/MAP/Tile_Foret_03.png" );
	AddTile( "../../resources/imgs/MAP/Tile_Prairie_01.png" );
	AddTile( "../../resources/imgs/MAP/Tile_Prairie_02.png" );
	AddTile( "../../resources/imgs/MAP/Ville_01.png" );
	AddTile( "../../resources/imgs/MAP/Ville_02.png" );

	// load all tiles
	for( int i=0; i<(int)m_oTileNames.size(); ++i )
	{
		SDL_Surface *tile = surfaceDB.LoadSurface( m_oTileNames[i] );

		if (tile != NULL)
		{
			m_oTileSurfaces.push_back( tile );
			if (tile->w < m_iMinTileWidth)
			{
				m_iMinTileWidth = tile->w;
			} 
			if (tile->h < m_iMinTileHeight)
			{
				m_iMinTileHeight = tile->h;
			}
		} 
	}

	// calculate tiles per line and tiles per row
	m_iTilesPerLine = iWidth / m_iMinTileWidth;
	if (iWidth % m_iMinTileWidth)
	{
		m_iTilesPerLine++;
	}
	m_iTilesPerColumn = iHeight / m_iMinTileHeight;
	if (iHeight % m_iMinTileHeight)
	{
		m_iTilesPerColumn++;
	}

	int rows = length;
	if (length % m_iMinTileHeight)
	{
		rows++;
	}

	//   cout << "Background: minTileWidth=" << minTileWidth << "  minTileHeight=" << minTileHeight << "  rows=" << rows << endl;

	int lastTile = rand() % (m_oTileSurfaces.size()-2);

	// generate random background
	for(int i= 0; i< rows; ++i )
	{
		if( lastTile == 0 )
			m_oTileNumbers.push_back( 1 );
		else if( lastTile == 1 )
			m_oTileNumbers.push_back( 1 + rand() % 2 );
		else if( lastTile == 2 )
			m_oTileNumbers.push_back( 3 + rand() % 2 );
		else
		{
			int tile = rand() % 3;
			if( tile != 0 ) tile += 2;
			m_oTileNumbers.push_back( tile );
		}

		lastTile = m_oTileNumbers.back();

		for( int j=1; j<m_iTilesPerLine; ++j )
		{
			if( lastTile < 3 )
				m_oTileNumbers.push_back( lastTile );
			else
				m_oTileNumbers.push_back( 3 + rand() % 2 );
		}
	}

	//generate ending
	for( int i=0; i<3 ; i++ )
	{
		if( lastTile == 0 )
			m_oTileNumbers.push_back( 1 );
		else if( lastTile == 1 )
			m_oTileNumbers.push_back( 2 );
		else
			m_oTileNumbers.push_back( 3 + rand() % 2 );

		lastTile = m_oTileNumbers.back();

		for( int j=1; j<m_iTilesPerLine; ++j )
		{
			if( lastTile < 3 )
				m_oTileNumbers.push_back( lastTile );
			else
				m_oTileNumbers.push_back( 3 + rand() % 2 );
		}
	}
}


void Background::Draw( SDL_Surface* screen )
{
	m_iStep = (m_iStep+1) % (m_iTilesPerColumn*m_iMinTileHeight);
	Draw( screen, m_iStep );
}

bool Background::Draw( SDL_Surface* screen, int step )
{
	bool bEnd = true;
	int iWidth = PrefsManager::GetInstance()->GetValue("SCREEN_WIDTH");
	int iHeight = PrefsManager::GetInstance()->GetValue("SCREEN_HEIGHT");

	if (step < 0)
	{
		step *= -1;
	}
	int startLine = (step / m_iMinTileHeight);
	int offset    = (step % m_iMinTileHeight);

	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;

	SDL_Rect dstRect;

	for(int y = 0; y < m_iTilesPerColumn+1; y++)
	{
		for(int x = 0; x < m_iTilesPerLine; x++)
		{
			int diffX = iWidth - x * m_iMinTileWidth;
			if ( diffX >= m_iMinTileWidth )
			{
				srcRect.w = m_iMinTileWidth;
			}
			else
			{
				srcRect.w = diffX;
			}
			dstRect.w = srcRect.w;
	
			if (y==0)
			{
				int diffY = -(offset - m_iMinTileHeight);
				srcRect.h = diffY;
			}
			else
			{
				srcRect.h = m_iMinTileHeight;
			}
			dstRect.h = srcRect.h;
			dstRect.x = x * m_iMinTileWidth;
			dstRect.y = iHeight + offset - (y+1) * m_iMinTileHeight;
			int currentTile = ((y+startLine)*m_iTilesPerLine+x);
			int tile = 0;
			if( currentTile > (int)m_oTileNumbers.size() - 1 )
				tile = 5 + currentTile%2;
			else
			{
				bEnd = false;
				tile = m_oTileNumbers[ currentTile ];
			}
			SDL_BlitSurface( m_oTileSurfaces[ tile ] , &srcRect, screen, &dstRect );
		}
	}
	return bEnd;
}

