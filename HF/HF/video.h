#ifndef VIDEO_H
#define VIDEO_H

#include "SDL.h"

class Video;

extern Video *videoserver;

class Video {

private:
	SDL_Surface *screen;

public:
	Video();
	~Video();
	SDL_Surface *init();

	void clearScreen();
	void toggleFullscreen();

	int m_iHeight;
	int m_iWidth;
	int m_iDepth;
	bool m_bFullScreen;
};

#endif //#ifndef VIDEO_HH
