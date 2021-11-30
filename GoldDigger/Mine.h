#ifndef __MINE_H__
#define __MINE_H__

#include "GameObject.h"

class Mine : public GameObject
{
public:
	Mine() : GameObject(Types::MINE, -1) {}
	void Initialise();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	static Sprite* m_sprites[];
};

#endif // !__MINE_H__