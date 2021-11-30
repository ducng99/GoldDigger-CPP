#ifndef __BACKGROUNDROCK_H__
#define __BACKGROUNDROCK_H__

#include "GameObject.h"

class BackgroundRock : public GameObject
{
public:
	BackgroundRock() : GameObject(Types::BGROCK, 0) {}
	void Initialise();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	static Sprite* m_sprites[];
};

#endif // !__BACKGROUNDROCK_H__