#ifndef PLANE_HH
#define PLANE_HH

#include "SDL.h"
#include "geometry.h"
#include <string>
#include <vector>

class SurfaceDB;
class Items;

/* The Racer is the vehicle, that the player can steer. */
class Plane
{
	SDL_Surface *spriteRacerBase;
	SDL_Rect drawRectBase;
	SDL_Surface *spriteShadow;

	std::vector< SDL_Surface* > m_oSpritesUp;
	std::vector< SDL_Surface* > m_oSpritesTurnLeft;
	std::vector< SDL_Surface* > m_oSpritesLeft;
	std::vector< SDL_Surface* > m_oSpritesTurnRight;
	std::vector< SDL_Surface* > m_oSpritesRight;

	std::vector< SDL_Surface* >* m_pCurrentAnim;

	int m_iCurrentSprite;

	// Movement-System
	Vector2D pos; // absolute position
	Vector2D vel; // the velocity vector
	Vector2D realSize;

	Uint32 timeLastMove;

	float maxVel;

public:

	enum PlayerEvent { PE_UNKNOWN = 0, PE_LEFT, PE_RIGHT };

	// Input-System - which keys are pressed at the moment
	void handlePlayerEvent( PlayerEvent pEvent, bool keyDown );

	Plane();
	~Plane();

	void setPos( const Vector2D &newPos );
	inline Vector2D getPos() { return pos; }
	inline Vector2D getVel() { return vel; }
	void setMaxVel( float newVel ); // returns old vel

	// moves the racer according to his velocity and the pressed keys. 
	// Already collides with the boundaryRect (the border of the world)
	Vector2D move( int dT );
	void clipWorld();

	void drawPlane( SDL_Surface *screen );
	void drawShadow( SDL_Surface *screen );
	void drawStats( SDL_Surface *screen );

	// picks up items, if flown over
	void pickUpItems();
	void UpdateAnim();

	bool left;
	bool right;

};

#endif //PLANE_HH
