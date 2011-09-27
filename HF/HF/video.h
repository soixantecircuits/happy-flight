#ifndef VIDEO_H
#define VIDEO_H

#include "SDL.h"
#include "Singleton.h"

class Video : public Singleton<Video>
{
	friend class Singleton<Video>;

private:
	SDL_Surface *m_pScreen;

public:
	Video();
	~Video();
	SDL_Surface *Init();

	void Flip();
	void DrawRect( int iTextureId, SDL_Rect* src, SDL_Rect* dst );
	void Close();

	int m_iGameHeight;
	int m_iGameWidth;
	int m_iWindowHeight;
	int m_iWindowWidth;
	int m_iDepth;
	bool m_bFullScreen;
};

#endif //#ifndef VIDEO_HH
