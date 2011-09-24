#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <vector>
#include <string>

struct SDL_Surface;

class Background
{
public:
	Background();

	void ClearTileList();
	void AddTile( std::string tilename );
	void GenerateBackground( int length );
	void Draw( SDL_Surface* screen );
	bool Draw( SDL_Surface* screen, int step );

private:

	int m_iMinTileWidth;
	int m_iMinTileHeight;
	int m_iTilesPerLine;
	int m_iTilesPerColumn;
	int m_iStep;

	std::vector< std::string > m_oTileNames;
	std::vector< SDL_Surface* > m_oTileSurfaces;
	std::vector< int > m_oTileNumbers;
};


#endif
