#ifndef __CHEST_H__
#define __CHEST_H__

#include "GameObject.h"

class Chest : public GameObject
{
public:
	Chest() : GameObject(Types::CHEST, 50) {}
	void Initialise();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();

	void OpenChest();
private:
	static Sprite* m_sprites[];
};

#endif // !__CHEST_H__