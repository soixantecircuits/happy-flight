using namespace std;

#include "surfaceDB.h"
#include "SDL_image.h"
#include "PrefsManager.h"
#include <fstream>
#include <iostream>

TextureManager::TextureManager()
{
	m_iCurrent = 0;
}

TextureManager::~TextureManager()
{
	std::map<int, SDL_Surface *>::iterator pos;
	// free all surfaces
	for ( pos = m_oTextureDB.begin(); pos != m_oTextureDB.end(); ++pos )
	{
		SDL_FreeSurface( pos->second );
	}
}

int TextureManager::LoadSurface( string fn, bool bAlpha )
{
	int id = GetTextureId( fn );
	if( id >= 0 )
	{
		return id;
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
		if( bAlpha )
			optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		else
			optimizedImage = SDL_DisplayFormat( loadedImage );
		
		//Free the old image
		SDL_FreeSurface( loadedImage );

		m_oIdDB[ fn ] = m_iCurrent;
		m_oTextureDB[ m_iCurrent ] = optimizedImage;

		return m_iCurrent++;
	}

	return -1;
}

int TextureManager::GetTextureId( string fn )
{
	if( m_oIdDB.empty() )
	{
		return -1;
	}
	else
	{
		std::map<std::string, int>::iterator pos = m_oIdDB.find( fn );
		if ( pos == m_oIdDB.end() )
		{
			return -1;
		}
		else
		{
			return pos->second;
		}
	}
}


SDL_Surface *TextureManager::GetTextureById( int id )
{
	if( m_oTextureDB.empty() )
	{
		return 0;
	}
	else
	{
		std::map<int, SDL_Surface *>::iterator pos = m_oTextureDB.find( id );
		if ( pos == m_oTextureDB.end() )
		{
			return 0;
		}
		else
		{
			return pos->second;
		}
	}
}

