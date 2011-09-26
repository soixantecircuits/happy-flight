#ifndef SURFACE_DB_HH
#define SURFACE_DB_HH

#include "SDL.h"
#include "Singleton.h"
#include <string>
#include <map>

#define ROUND(x) ((float)floor( x + 0.5 ))

class TextureManager : public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;

public:
	TextureManager();
	~TextureManager();

	int LoadSurface( std::string fn, bool bAlpha = true );

	SDL_Surface *GetTextureById( int id );
	int GetTextureId( std::string fn );

private:
	std::map<std::string, int> m_oIdDB;
	std::map<int, SDL_Surface *> m_oTextureDB;
	int m_iCurrent;
};


#endif //#ifndef SURFACE_DB_HH
