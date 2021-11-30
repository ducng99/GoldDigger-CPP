#ifndef __DIRT_H__
#define __DIRT_H__

#include "GameObject.h"

class Sprite;

class Dirt : public GameObject
{
public:
	enum DirtPosition
	{
		TOP_LEFT, TOP_LEFT_DIGGED,
		TOP, TOP_DIGGED,
		TOP_RIGHT, TOP_RIGHT_DIGGED,
		MIDDLE_LEFT, MIDDLE_LEFT_DIGGED,
		MIDDLE, MIDDLE_DIGGED,
		MIDDLE_RIGHT, MIDDLE_RIGHT_DIGGED
	};

	Dirt() : GameObject(Types::DIRT, 0) {}

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();

	void Initialise(DirtPosition position);
	DirtPosition GetDirtPosition();
private:
	void Initialise(Sprite* sprite) {}
private:
	DirtPosition m_position;
	static Sprite* m_sprites[];
};

#endif // !__DIRT_H__