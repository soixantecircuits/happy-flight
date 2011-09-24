#ifndef VIDEO_H
#define VIDEO_H

#include "SDL.h"

class Video;

extern Video *g_pVideoserver;

class Video {

private:
	SDL_Surface *m_pScreen;

public:
	Video();
	~Video();
	SDL_Surface *Init();

	void ClearScreen();
	void ToggleFullscreen();

	int m_iHeight;
	int m_iWidth;
	int m_iDepth;
	bool m_bFullScreen;
};

#endif //#ifndef VIDEO_HH
