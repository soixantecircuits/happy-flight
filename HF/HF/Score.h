#ifndef SCORE_H
#define SCORE_H

#include "SDL.h"
#include "geometry.h"
#include <string>
#include <vector>

class Score
{
	std::vector< int > m_oSprites;
	SDL_Surface* m_pScoreSprite;
	SDL_Surface* m_pNumberSprite;
	int m_iScore;

public:
	Score();
	~Score();

	void SetScore( int iValue );
	void Draw(SDL_Surface *screen);
};

#endif
