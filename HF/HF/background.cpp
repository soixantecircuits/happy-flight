#include "background.h"
#include "surfaceDB.h"
#include "SDL.h"
#include <iostream>
#include "PrefsManager.h"

using namespace std;

Background::Background()
{
	minTileWidth   = 9999999;
	minTileHeight  = 9999999;
	tilesPerLine   = 0;
	tilesPerColumn = 0;
	step           = 0;
}


void Background::clearTileList()
{
	tileNames.clear();
	tilesPerLine   = 0;
	tilesPerColumn = 0;
}


void Background::addTile( string tilename )
{
	tileNames.push_back( tilename );
}


void Background::generateBackground( int length )
{
	int iWidth = PrefsManager::GetInstance()->GetValue("SCREEN_WIDTH");
	int iHeight = PrefsManager::GetInstance()->GetValue("SCREEN_HEIGHT");

	tileSurfaces.clear();
	minTileWidth   = 9999999;
	minTileHeight  = 9999999;

	addTile( "..\\..\\resources\\imgs\\MAP\\Tile_Foret_01.png" );
	addTile( "..\\..\\resources\\imgs\\MAP\\Tile_Foret_02.png" );
	addTile( "..\\..\\resources\\imgs\\MAP\\Tile_Foret_03.png" );
	addTile( "..\\..\\resources\\imgs\\MAP\\Tile_Prairie_01.png" );
	addTile( "..\\..\\resources\\imgs\\MAP\\Tile_Prairie_02.png" );
	addTile( "..\\..\\resources\\imgs\\MAP\\Ville_01.png" );
	addTile( "..\\..\\resources\\imgs\\MAP\\Ville_02.png" );

	// load all tiles
	for( int i=0; i<(int)tileNames.size(); ++i )
	{
		SDL_Surface *tile = surfaceDB.loadSurface( tileNames[i] );

		if (tile != NULL)
		{
			tileSurfaces.push_back( tile );
			if (tile->w < minTileWidth)
			{
				minTileWidth = tile->w;
			} 
			if (tile->h < minTileHeight)
			{
				minTileHeight = tile->h;
			}
		} 
	}

	// calculate tiles per line and tiles per row
	tilesPerLine = iWidth / minTileWidth;
	if (iWidth % minTileWidth)
	{
		tilesPerLine++;
	}
	tilesPerColumn = iHeight / minTileHeight;
	if (iHeight % minTileHeight)
	{
		tilesPerColumn++;
	}

	int rows = length;
	if (length % minTileHeight)
	{
		rows++;
	}

	//   cout << "Background: minTileWidth=" << minTileWidth << "  minTileHeight=" << minTileHeight << "  rows=" << rows << endl;

	int lastTile = rand() % (tileSurfaces.size()-2);

	// generate random background
	for(int i= 0; i< rows; ++i )
	{
		if( lastTile == 0 )
			tileNumbers.push_back( 1 );
		else if( lastTile == 1 )
			tileNumbers.push_back( 1 + rand() % 2 );
		else if( lastTile == 2 )
			tileNumbers.push_back( 3 + rand() % 2 );
		else
		{
			int tile = rand() % 3;
			if( tile != 0 ) tile += 2;
			tileNumbers.push_back( tile );
		}

		lastTile = tileNumbers.back();

		for( int j=1; j<tilesPerLine; ++j )
		{
			if( lastTile < 3 )
				tileNumbers.push_back( lastTile );
			else
				tileNumbers.push_back( 3 + rand() % 2 );
		}
	}

	//generate ending
	for( int i=0; i<3 ; i++ )
	{
		if( lastTile == 0 )
			tileNumbers.push_back( 1 );
		else if( lastTile == 1 )
			tileNumbers.push_back( 2 );
		else
			tileNumbers.push_back( 3 + rand() % 2 );

		lastTile = tileNumbers.back();

		for( int j=1; j<tilesPerLine; ++j )
		{
			if( lastTile < 3 )
				tileNumbers.push_back( lastTile );
			else
				tileNumbers.push_back( 3 + rand() % 2 );
		}
	}
}


void Background::draw( SDL_Surface* screen )
{
	step = (step+1) % (tilesPerColumn*minTileHeight);
	draw( screen, step );
}

void Background::draw( SDL_Surface* screen, int step )
{
	int iWidth = PrefsManager::GetInstance()->GetValue("SCREEN_WIDTH");
	int iHeight = PrefsManager::GetInstance()->GetValue("SCREEN_HEIGHT");

	if (step < 0)
	{
		step *= -1;
	}
	int startLine = (step / minTileHeight);
	int offset    = (step % minTileHeight);

	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;

	SDL_Rect dstRect;

	for(int y = 0; y < tilesPerColumn+1; y++)
	{
		for(int x = 0; x < tilesPerLine; x++)
		{
			int diffX = iWidth - x * minTileWidth;
			if ( diffX >= minTileWidth )
			{
				srcRect.w = minTileWidth;
			}
			else
			{
				srcRect.w = diffX;
			}
			dstRect.w = srcRect.w;
	
			if (y==0)
			{
				int diffY = -(offset - minTileHeight);
				srcRect.h = diffY;
			}
			else
			{
				srcRect.h = minTileHeight;
			}
			dstRect.h = srcRect.h;
			dstRect.x = x * minTileWidth;
			dstRect.y = iHeight + offset - (y+1) * minTileHeight;
			int currentTile = ((y+startLine)*tilesPerLine+x);
			int tile = 0;
			if( currentTile > (int)tileNumbers.size() - 1 )
				tile = 5 + currentTile%2;
			else
				tile = tileNumbers[ currentTile ];
			SDL_BlitSurface( tileSurfaces[ tile ] , &srcRect, screen, &dstRect );
		}
	}
}

