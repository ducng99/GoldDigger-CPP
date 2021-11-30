#ifndef __DIAMOND_H__
#define __DIAMOND_H__

#include "GameObject.h"

class Diamond : public GameObject
{
public:
	Diamond() : GameObject(Types::DIAMOND, 20) {}
	void Initialise();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	static Sprite* m_sprites[];
};

#endif // !__DIAMOND_H__