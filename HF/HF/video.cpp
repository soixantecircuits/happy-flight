#include "Video.h"
#include "SDL.h"
#include <stdlib.h>
#include "TextureManager.h"
#include "PrefsManager.h"

#ifdef OPENGL
#include <GL/glut.h>
#endif

using namespace std;

#ifdef OPENGL

/* OpenGL "state optimizer" hack from glSDL */
static struct
{
	int	do_blend;
	int	do_texture;
	GLint	texture;
	GLenum	sfactor, dfactor;
} glstate;

static void gl_reset(void)
{
	glstate.do_blend = -1;
	glstate.texture = -1;
	glstate.sfactor = 0xffffffff;
	glstate.dfactor = 0xffffffff;
}

static __inline__ void gl_do_blend(int on)
{
	if(glstate.do_blend == on)
		return;

	if(on)
	{
		glEnable(GL_BLEND);
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}
	else
		glDisable(GL_BLEND);
	glstate.do_blend = on;
}

static __inline__ void gl_do_texture(int on)
{
	if(glstate.do_texture == on)
		return;

	if(on)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	glstate.do_texture = on;
}

static __inline__ void gl_texture(GLuint tx)
{
	if(tx == glstate.texture)
		return;

	glBindTexture(GL_TEXTURE_2D, tx);
	glstate.texture = tx;
}
SDL_Surface *tmp, *tmp2;
GLuint gl_tiles;
#endif

Video::Video()
{
	m_pScreen = 0;
	m_iGameWidth = PrefsManager::GetInstance()->GetValue( "GAME_WIDTH" );
	m_iGameHeight = PrefsManager::GetInstance()->GetValue( "GAME_HEIGHT" );
	m_iWindowWidth = PrefsManager::GetInstance()->GetValue( "WINDOW_WIDTH" );
	m_iWindowHeight = PrefsManager::GetInstance()->GetValue( "WINDOW_HEIGHT" );
	m_iDepth = PrefsManager::GetInstance()->GetValue( "BIT_DEPTH" );
	m_bFullScreen = PrefsManager::GetInstance()->GetValue( "FULL_SCREEN" )==0?false:true;
}

Video::~Video()
{
  // kill something
}

void Video::Flip()
{
#ifdef OPENGL
	SDL_GL_SwapBuffers();
#else
	SDL_Flip( m_pScreen );
#endif
}

void Video::Close()
{
#ifdef OPENGL
	//glDeleteTextures(1, &tex);
#endif
	SDL_Quit();
}

SDL_Surface *Video::Init()
{
#ifdef OPENGL

	GLint gl_doublebuf;
	int flags = 0;
	flags |= SDL_DOUBLEBUF;
	flags |= SDL_OPENGL;
	if( m_bFullScreen )
		flags |= SDL_FULLSCREEN;

	gl_doublebuf = flags & SDL_DOUBLEBUF;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_iDepth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, gl_doublebuf);

	m_pScreen = SDL_SetVideoMode( m_iWindowWidth, m_iWindowHeight, m_iDepth, flags );
	if (!m_pScreen)
	{
		printf("Couldn't set %dx%d, %dbit video mode: %s\n", m_iWindowWidth, m_iWindowHeight, m_iDepth, SDL_GetError());
		exit(2);
	}

	//Set up OpenGL for 2D rendering.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport( 0, 0, m_iWindowWidth, m_iWindowHeight );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, m_iGameWidth, m_iGameHeight, 0, -1.0, 1.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, 0.0f );

	gl_reset();

#else
	// --------------------------------------------------
	// SDL initialisation
	// -----------------------------------------------------

	if( m_bFullScreen )
		m_pScreen = SDL_SetVideoMode( m_iGameWidth, m_iGameHeight, m_iDepth, SDL_DOUBLEBUF | SDL_FULLSCREEN | SDL_HWSURFACE );
	else
		m_pScreen = SDL_SetVideoMode( m_iGameWidth, m_iGameHeight, m_iDepth, SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (!m_pScreen)
	{
		printf("Couldn't set %dx%d, %dbit video mode: %s\n", m_iGameWidth, m_iGameHeight, m_iDepth, SDL_GetError());
		exit(2);
	}

#endif
	SDL_WM_SetCaption("HappyFlight", "HappyFlight");
	//SDL_WM_SetIcon(SDL_LoadBMP( FN_ALIENBLASTER_ICON.c_str() ), NULL);
	//SDL_ShowCursor(SDL_DISABLE);

	return m_pScreen;
}

void Video::DrawRect( int iTextureId, SDL_Rect* src, SDL_Rect* dst )
{
#ifdef OPENGL
	float tx1 = 0;
	float ty1 = 0;
	float tx2 = 1;
	float ty2 = 1;

	if( src )
	{
		SDL_Surface* pSurface = TextureManager::GetInstance()->GetTextureById( iTextureId );
		tx1 = (float)src->x / pSurface->w;
		ty1 = (float)src->y / pSurface->h;
		tx2 = tx1 + (float)src->w / pSurface->w;
		ty2 = ty1 + (float)src->h / pSurface->h;
	}

	float x1 = dst->x;
	float y1 = dst->y;
	float x2 = dst->x + dst->w;
	float y2 = dst->y + dst->h;

	gl_do_texture(1);
	gl_do_blend(1);
	gl_texture(iTextureId);

	glBegin(GL_QUADS);
	glColor4ub(255, 255, 255, 255);
	glTexCoord2f(tx1, ty1);	glVertex2f(x1, y1);
	glTexCoord2f(tx2, ty1);	glVertex2f(x2, y1);
	glTexCoord2f(tx2, ty2);	glVertex2f(x2, y2);
	glTexCoord2f(tx1, ty2);	glVertex2f(x1, y2);
	glEnd();
#else
	SDL_BlitSurface( TextureManager::GetInstance()->GetTextureById( iTextureId ), src, m_pScreen, dst );
#endif
}
