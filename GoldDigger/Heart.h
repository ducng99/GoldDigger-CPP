#ifndef __HEART_H__
#define __HEART_H__

#include "GameObject.h"

class Heart : public GameObject
{
public:
	Heart() : GameObject(Types::HEART, 1) {}
	void Initialise();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	static Sprite* m_sprites[];
};

#endif // !__HEART_H__