#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "SDL.h"
#include "geometry.h"

struct MyRect
{
	int x, y, w, h;
};

class BoundingBox
{

private:
	MyRect box;

public:
	BoundingBox(int x, int y, int width, int height);
	int getUpperBound();
	int getLowerBound();
	int getLeftBound();
	int getRightBound();
	bool overlaps(BoundingBox *other);
	bool overlaps(const Vector2D &startOfLine, const Vector2D &endOfLine);
	bool overlaps(const Circle &circle);
	void modifyX(int value);
	void modifyY(int value);
	void moveUpperBound(int upperBound);
	void moveLowerBound(int lowerBound);
	void moveLeftBound(int leftBound);
	void moveRightBound(int rightBound);
	//	SDL_Rect *getRect();
};

#endif
