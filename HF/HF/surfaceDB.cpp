using namespace std;

#include "surfaceDB.h"
#include "SDL_image.h"
#include <fstream>
#include <iostream>

SurfaceDB surfaceDB;


SurfaceDB::SurfaceDB()
{
}

SurfaceDB::~SurfaceDB()
{
	StringSurfaceMap::iterator pos;
	// free all surfaces
	for ( pos = surfaceDB.begin(); pos != surfaceDB.end(); ++pos )
	{
		SDL_FreeSurface( pos->second );
	}
}

SDL_Surface *SurfaceDB::LoadSurface( string fn )
{
	SDL_Surface *searchResult = GetSurface( fn );
	if ( searchResult )
	{
		return searchResult;
	}

	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image using SDL_image
	loadedImage = IMG_Load( fn.c_str() );

	//If the image loaded
	if( loadedImage != NULL )
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		//Free the old image
		SDL_FreeSurface( loadedImage );
	}

	surfaceDB[ fn ] = optimizedImage;
	return optimizedImage;
}

SDL_Surface *SurfaceDB::GetSurface( string fn )
{
	if( surfaceDB.empty() )
	{
		return 0;
	}
	else
	{
		StringSurfaceMap::iterator pos = surfaceDB.find( fn );
		if ( pos == surfaceDB.end() )
		{
			return 0;
		}
		else
		{
			return pos->second;
		}
	}
}

