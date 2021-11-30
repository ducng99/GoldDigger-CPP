#ifndef __GOLD_H__
#define __GOLD_H__

#include "GameObject.h"

class Gold : public GameObject
{
public:
	Gold() : GameObject(Types::GOLD, 10) {}
	void Initialise();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	static Sprite* m_sprites[];
};

#endif // !__GOLD_H__