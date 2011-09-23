#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <vector>
#include <string>

struct SDL_Surface;

class Background {
	public:
	Background();
  
	void clearTileList();
	void addTile( std::string tilename );
	void generateBackground( int length );
	void draw( SDL_Surface* screen );
	void draw( SDL_Surface* screen, int step );

	private:

	int minTileWidth;
	int minTileHeight;
	int tilesPerLine;
	int tilesPerColumn;
	int step;

	std::vector< std::string > tileNames;
	std::vector< SDL_Surface* > tileSurfaces;
	std::vector< int > tileNumbers;
};


#endif
