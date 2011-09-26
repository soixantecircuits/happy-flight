#ifndef PLANE_HH
#define PLANE_HH

#include "SDL.h"
#include "geometry.h"
#include <string>
#include <vector>

class TextureManager;
class Items;
class Game;

/* The Racer is the vehicle, that the player can steer. */
class Plane
{
	int m_iSpriteBase;
	SDL_Surface* m_pSpriteBase;
	SDL_Rect m_oDrawRectBase;

	std::vector< int > m_oSpritesUp;
	std::vector< int > m_oSpritesTurnLeft;
	std::vector< int > m_oSpritesLeft;
	std::vector< int > m_oSpritesTurnRight;
	std::vector< int > m_oSpritesRight;

	std::vector< int >* m_pCurrentAnim;

	int m_iCurrentSprite;

	// Movement-System
	Vector2D m_vPos; // absolute position
	Vector2D m_vVel; // the velocity vector
	Vector2D m_vRealSize;

	Uint32 m_iTimeLastMove;
	int m_iScreenWidth;
	int m_iScreenHeight;
	int m_iAnimDelay;
	int m_iPickDistance;
	int m_iPlaneDist;
	int m_iPlaneWidth;
	int m_iPlaneHeight;

	float m_fMaxVel;
	bool m_bLeft;
	bool m_bRight;

	Items* m_pItems;
	Game* m_pGame;

public:
	Plane( Items* pItems, Game* pGame );
	~Plane();

	void ReloadConfig();
	void SetPos( const Vector2D &newPos );
	inline Vector2D GetPos() { return m_vPos; }
	inline Vector2D GetVel() { return m_vVel; }
	void SetMaxVel( float newVel ); // returns old vel

	// moves the racer according to his velocity and the pressed keys. 
	// Already collides with the boundaryRect (the border of the world)
	Vector2D Move( int dT );
	void ClipWorld();

	void DrawPlane( SDL_Surface *screen );

	// picks up items, if flown over
	void PickUpItems();
	void UpdateAnim();

	void GoLeft(){ m_bLeft = true; }
	void GoRight(){ m_bRight = true; }

};

#endif //PLANE_HH
