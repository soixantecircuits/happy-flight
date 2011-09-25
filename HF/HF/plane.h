#ifndef PLANE_HH
#define PLANE_HH

#include "SDL.h"
#include "geometry.h"
#include <string>
#include <vector>

class SurfaceDB;
class Items;
class Game;

/* The Racer is the vehicle, that the player can steer. */
class Plane
{
	SDL_Surface *m_pSpriteBase;
	SDL_Rect m_oDrawRectBase;

	std::vector< SDL_Surface* > m_oSpritesUp;
	std::vector< SDL_Surface* > m_oSpritesTurnLeft;
	std::vector< SDL_Surface* > m_oSpritesLeft;
	std::vector< SDL_Surface* > m_oSpritesTurnRight;
	std::vector< SDL_Surface* > m_oSpritesRight;

	std::vector< SDL_Surface* >* m_pCurrentAnim;

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

	float m_fMaxVel;
	bool m_bLeft;
	bool m_bRight;

	Items* m_pItems;
	Game* m_pGame;

public:
	Plane( Items* pItems, Game* pGame );
	~Plane();

	void SetPos( const Vector2D &newPos );
	inline Vector2D GetPos() { return m_vPos; }
	inline Vector2D GetVel() { return m_vVel; }
	void SetMaxVel( float newVel ); // returns old vel

	// moves the racer according to his velocity and the pressed keys. 
	// Already collides with the boundaryRect (the border of the world)
	Vector2D Move( int dT );
	void ClipWorld();

	void DrawPlane( SDL_Surface *screen );
	void DrawStats( SDL_Surface *screen );

	// picks up items, if flown over
	void PickUpItems();
	void UpdateAnim();

	void GoLeft(){ m_bLeft = true; }
	void GoRight(){ m_bRight = true; }

};

#endif //PLANE_HH
