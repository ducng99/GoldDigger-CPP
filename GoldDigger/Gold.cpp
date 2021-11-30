#include "Gold.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* Gold::m_sprites[1];

void Gold::Initialise()
{
	m_pSprite = m_sprites[0];
}

void Gold::InitialiseSprites(BackBuffer * backBuffer)
{
	m_sprites[0] = backBuffer->CreateSprite("assets\\imgs\\Objects.png", 0, 0, 80, 80);
}

void Gold::DestroySprites()
{
	delete m_sprites[0];
	m_sprites[0] = 0;
}
