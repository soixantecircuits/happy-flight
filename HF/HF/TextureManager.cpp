using namespace std;

#include "TextureManager.h"
#include "SDL_image.h"
#include "PrefsManager.h"
#include <fstream>
#include <iostream>

#ifdef OPENGL
#include <GL/glut.h>
#endif

TextureManager::TextureManager()
{
	m_iCurrent = 1;
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
	if( id > 0 )
	{
		return id;
	}

	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image using SDL_image
	loadedImage = IMG_Load( fn.c_str() );

#ifdef OPENGL
	int bpp = bAlpha?32:24;
	SDL_Surface *tmp, *tmp2;
	tmp = loadedImage;
	tmp2 = SDL_CreateRGBSurface(SDL_SWSURFACE, loadedImage->w, loadedImage->h, bpp, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 );

	SDL_BlitSurface(tmp, NULL, tmp2, NULL);

	GLuint txid;
	glGenTextures(1, &txid);
	glBindTexture(GL_TEXTURE_2D, txid);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tmp2->pitch / tmp2->format->BytesPerPixel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if( bAlpha )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tmp2->w, tmp2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp2->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, tmp2->w, tmp2->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tmp2->pixels);
	
	glFlush();
	SDL_FreeSurface(tmp2);

	m_oIdDB[ fn ] = txid;
	m_oTextureDB[ txid ] = loadedImage;

	return txid;
#else
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
#endif

	return 0;
}

int TextureManager::GetTextureId( string fn )
{
	if( m_oIdDB.empty() )
	{
		return 0;
	}
	else
	{
		std::map<std::string, int>::iterator pos = m_oIdDB.find( fn );
		if ( pos == m_oIdDB.end() )
		{
			return 0;
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

