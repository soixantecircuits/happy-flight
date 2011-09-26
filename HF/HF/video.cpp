#include "Video.h"
#include "SDL.h"
#include <stdlib.h>
#include "TextureManager.h"
#include "PrefsManager.h"

using namespace std;

Video::Video()
{
	m_pScreen = 0;
	m_iWidth = PrefsManager::GetInstance()->GetValue( "SCREEN_WIDTH" );
	m_iHeight = PrefsManager::GetInstance()->GetValue( "SCREEN_HEIGHT" );
	m_iDepth = PrefsManager::GetInstance()->GetValue( "BIT_DEPTH" );
	m_bFullScreen = PrefsManager::GetInstance()->GetValue( "FULL_SCREEN" )==0?false:true;
}

Video::~Video()
{
  // kill something
}

void Video::Flip()
{
	SDL_Flip( m_pScreen );
}

void Video::Close()
{
	SDL_Quit();
}

SDL_Surface *Video::Init()
{
	// --------------------------------------------------
	// SDL initialisation
	// -----------------------------------------------------
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL video subsystem: %s\n", SDL_GetError());
		exit(1);
	}

	if( m_bFullScreen )
		m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF | SDL_FULLSCREEN | SDL_HWSURFACE );
	else
		m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (!m_pScreen)
	{
		printf("Couldn't set %dx%d, %dbit video mode: %s\n", m_iWidth, m_iHeight, m_iDepth, SDL_GetError());
		exit(2);
	}

	SDL_WM_SetCaption("HappyFlight", "HappyFlight");
	//SDL_WM_SetIcon(SDL_LoadBMP( FN_ALIENBLASTER_ICON.c_str() ), NULL);
	//SDL_ShowCursor(SDL_DISABLE);

	return m_pScreen;
}


void Video::ClearScreen()
{
	// clear the screen
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = m_pScreen->w;
	r.h = m_pScreen->h;
	SDL_FillRect(m_pScreen, &r, SDL_MapRGB(m_pScreen->format, 0, 0, 0) );
}

void Video::ToggleFullscreen()
{
	if ( m_bFullScreen )
	{
		m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF | SDL_HWSURFACE );
	}
	else
	{
		m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF | SDL_FULLSCREEN | SDL_HWSURFACE );
	}
	m_bFullScreen = !m_bFullScreen;
}

void Video::DrawRect( int iTextureId, SDL_Rect* src, SDL_Rect* dst )
{
	SDL_BlitSurface( TextureManager::GetInstance()->GetTextureById( iTextureId ), src, m_pScreen, dst );
}
