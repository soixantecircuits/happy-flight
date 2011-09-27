#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <vector>
#include <string>

struct SDL_Surface;

enum BackgroundState
{
	E_START = 0,
	E_TAKEOFF,
	E_FLYING,
	E_LANDING,
	E_STOP
};

class Background
{
public:
	Background();

	void ClearTileList();
	void AddTile( std::string tilename );
	void GenerateBackground( int length );
	void Draw( SDL_Surface* screen );
	void Draw( SDL_Surface* screen, int step );
	int GetState(){ return m_eBackgroundState; }
	void SetState( int iState ){ m_eBackgroundState = iState; }

private:

	int m_iMinTileWidth;
	int m_iMinTileHeight;
	int m_iTilesPerLine;
	int m_iTilesPerColumn;
	int m_iStep;

	int m_iWidth;
	int m_iHeight;

	int m_eBackgroundState;

	std::vector< std::string > m_oTileNames;
	std::vector< int > m_oTileIds;
	std::vector< int > m_oTileNumbers;
};


#endif
