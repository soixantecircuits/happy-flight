#ifndef SURFACE_DB_HH
#define SURFACE_DB_HH


#include "SDL.h"

#include <string>
#include <map>

typedef std::map<std::string, SDL_Surface *, std::greater<std::string> > StringSurfaceMap;


class SurfaceDB;

extern SurfaceDB surfaceDB;

float lroundf( float value );

/* Framework for convenient loading and management of SDL_Surfaces.
   Every surface has a transparent color (default violet).
   If the requested surface was already loaded it is not loaded again, 
   instead the old surface is returned.
   The clients of the surface may use the surface for blitting, but they must
   not change (especially delete) it.
*/
class SurfaceDB
{
public:
	SurfaceDB();
	~SurfaceDB();

	SDL_Surface *LoadSurface( std::string fn );

	private:
	StringSurfaceMap surfaceDB;

	SDL_Surface *GetSurface( std::string fn );
};


#endif //#ifndef SURFACE_DB_HH
