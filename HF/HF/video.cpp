#include "video.h"
#include "SDL.h"
#include <stdlib.h>
#include "PrefsManager.h"

using namespace std;

Video *videoserver;

Video::Video()
{
	screen = 0;
	m_iWidth = PrefsManager::GetInstance()->GetValue( "SCREEN_WIDTH" );
	m_iHeight = PrefsManager::GetInstance()->GetValue( "SCREEN_HEIGHT" );
	m_iDepth = PrefsManager::GetInstance()->GetValue( "BIT_DEPTH" );
	m_bFullScreen = PrefsManager::GetInstance()->GetValue( "FULL_SCREEN" )==0?false:true;
}

Video::~Video(){
  // kill something
}

SDL_Surface *Video::init(){
  // --------------------------------------------------
  // SDL initialisation
  // -----------------------------------------------------
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL video subsystem: %s\n", SDL_GetError());
    exit(1);
  }
  screen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF /* | SDL_FULLSCREEN */ );
  if (!screen) {
    printf("Couldn't set %dx%d, %dbit video mode: %s\n", m_iWidth, m_iHeight, m_iDepth, SDL_GetError());
    exit(2);
  }
  
  SDL_WM_SetCaption("HappyFlight", "HappyFlight");
  //SDL_WM_SetIcon(SDL_LoadBMP( FN_ALIENBLASTER_ICON.c_str() ), NULL);
  SDL_ShowCursor(SDL_DISABLE);

  return screen;
}


void Video::clearScreen()
{
	// clear the screen
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = screen->w;
	r.h = screen->h;
	SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 0, 0, 0) );
}

void Video::toggleFullscreen()
{
	if ( m_bFullScreen )
	{
		screen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF );
	}
	else
	{
		screen = SDL_SetVideoMode( m_iWidth, m_iHeight, m_iDepth, SDL_DOUBLEBUF | SDL_FULLSCREEN );
	}
	m_bFullScreen = !m_bFullScreen;
}
