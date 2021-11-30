#include "Mine.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* Mine::m_sprites[1];

void Mine::InitialiseSprites(BackBuffer * backBuffer)
{
	m_sprites[0] = backBuffer->CreateSprite("assets\\imgs\\Objects.png", 160, 0, 80, 80);
}

void Mine::DestroySprites()
{
	delete m_sprites[0];
	m_sprites[0] = 0;
}

void Mine::Initialise()
{
	m_pSprite = m_sprites[0];
}
